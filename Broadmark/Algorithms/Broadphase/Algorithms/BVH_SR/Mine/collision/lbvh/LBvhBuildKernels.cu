#include "LBvhKernels.cuh"
#include <cuda_runtime.h>
#include "utility\CudaDeviceUtils.h"
#include "BvhExtNode.h"
#include "BvhIntNode.h"
#include "setting\BvhSettings.h"

namespace mn {



	__global__ void calcMC64s(int size, int3* _faces, PointType* _vertices, BOX* scene, uint64* codes) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		BOX bv{};
		auto v = _vertices[_faces[idx].x];
		bv.combines(v.x, v.y, v.z);
		v = _vertices[_faces[idx].y];
		bv.combines(v.x, v.y, v.z);
		v = _vertices[_faces[idx].z];
		bv.combines(v.x, v.y, v.z);
		const PointType c = bv.center();
		const PointType offset = c - scene->_min;
		codes[idx] = morton3D64(offset.x / scene->width(), offset.y / scene->height(), offset.z / scene->depth());
	}

	__global__ void copyBackCodes(int size, uint64* _primcodes, uint* _codes) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		_primcodes[idx] = _codes[idx] << 32;
	}


	__global__ void buildIntNodes(int size, uint *_depths, BvhExtNodeCompletePort _lvs, BvhIntNodeCompletePort _tks) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		_lvs.lca(idx) = -1, _depths[idx] = 0;
		int		l = idx - 1, r = idx;	///< (l, r]
		bool	mark;

		if (l >= 0)	mark = _lvs.getmetric(l) < _lvs.getmetric(r);	///< true when right child, false otherwise
		else		mark = false;

		int		cur = mark ? l : r;
		_lvs.par(idx) = cur;
		if (mark)	_tks.rc(cur) = idx, _tks.rangey(cur) = idx, atomicOr(&_tks.mark(cur), 0x00000002), _lvs.mark(idx) = 0x00000007;
		else		_tks.lc(cur) = idx, _tks.rangex(cur) = idx, atomicOr(&_tks.mark(cur), 0x00000001), _lvs.mark(idx) = 0x00000003;

		while (atomicAdd(&_tks.flag(cur), 1) == 1) {
			//_tks.update(cur, _lvs);	/// Update
			_tks.refit(cur, _lvs);	/// Refit
			_tks.mark(cur) &= 0x00000007;

			l = _tks.rangex(cur) - 1, r = _tks.rangey(cur);
			_lvs.lca(l + 1) = cur/*, _tks.rcd(cur) = ++_lvs.rcl(r)*/, _depths[l + 1]++;
			if (l >= 0)	mark = _lvs.metric(l) < _lvs.metric(r);	///< true when right child, false otherwise
			else		mark = false;

			if (l + 1 == 0 && r == size - 1) {
				_tks.par(cur) = -1;
				_tks.mark(cur) &= 0xFFFFFFFB;
				break;
			}

			int par = mark ? l : r;
			_tks.par(cur) = par;
			if (mark)	_tks.rc(par) = cur, _tks.rangey(par) = r    , atomicAnd(&_tks.mark(par), 0xFFFFFFFD), _tks.mark(cur) |= 0x00000004;
			else		_tks.lc(par) = cur, _tks.rangex(par) = l + 1, atomicAnd(&_tks.mark(par), 0xFFFFFFFE), _tks.mark(cur) &= 0xFFFFFFFB;
			cur = par;
		}
	}

	__global__ void calcIntNodeOrders(int size, BvhIntNodeCompletePort _tks, int* _lcas, uint* _depths, uint* _offsets, int* _tkMap) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		//for (; idx < size; idx += gridDim.x * blockDim.x) {
		int node = _lcas[idx], depth = _depths[idx], id = _offsets[idx];
		if (node != -1) {
			for (; depth--; node = _tks.lc(node)) {
				_tkMap[node] = id++;
			}
		}
		//}
	}

	__global__ void updateBvhExtNodeLinks(int size, const int *_mapTable, int* _lcas, int* _pars) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		int ori;
		_pars[idx] = _mapTable[_pars[idx]];
		if ((ori = _lcas[idx]) != -1)
			_lcas[idx] = _mapTable[ori] << 1;
		else
			_lcas[idx] = idx << 1 | 1;
		//if (_lvs.getrca(idx - (size - 1)) != -1)
		//	_lvs.rca(idx - (size - 1)) = _mapTable[_lvs.getrca(idx - (size - 1))] << 1;
		//else
		//	_lvs.rca(idx - (size - 1)) = idx - (size - 1) << 1 | 1;
	}

	__global__ void reorderIntNode(int intSize, const int* _tkMap, BvhIntNodeCompletePort _unorderedTks, BvhIntNodeCompletePort _tks) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= intSize) return;
		int newId = _tkMap[idx];
		uint mark = _unorderedTks.getmark(idx);

		_tks.lc(newId) = mark & 1 ? _unorderedTks.getlc(idx) : _tkMap[_unorderedTks.getlc(idx)];
		_tks.rc(newId) = mark & 2 ? _unorderedTks.getrc(idx) : _tkMap[_unorderedTks.getrc(idx)];
		_tks.mark(newId) = mark;
		mark = _unorderedTks.getpar(idx);
		_tks.par(newId) = mark != -1 ? _tkMap[mark] : -1;
		_tks.rangex(newId) = _unorderedTks.getrangex(idx);
		_tks.rangey(newId) = _unorderedTks.getrangey(idx);
		//_tks.rcd(newId) = _rcls[mark] - _unorderedTks.getrcd(idx);
		_tks.setBV(newId, _unorderedTks, idx);
	}

	__global__ void checkRestrTrunkMap(int numRtIntNode, const int* _restrQueue, const int* _tkMap, const int * _restrIntMark, int *_rtIntCount) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= numRtIntNode) return;
		int mappedVal = _tkMap[_restrQueue[idx]];
		if (atomicAdd(_rtIntCount + mappedVal, 1) != 0)
			printf("BVH_SR: \t \n\t~~%d-th index(%d) mapped int index(%d) repeated\n", idx, _restrQueue[idx], mappedVal);
		if (_restrIntMark[mappedVal] == 0)
			printf("BVH_SR: \t \n\t~~%d-th index(%d) mapped int index(%d) not requiring restructuring\n", idx, _restrQueue[idx], mappedVal);
	}
	__global__ void checkPrimmap(int size, int* _primmap, int* _cnt) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		int val;
		if ((val = atomicAdd(&_cnt[_primmap[idx]], 1)) != 0)
			printf("BVH_SR: \t %d-th map record(%d) wrong %d\n", idx, _primmap[idx], val);
	}
	__global__ void checkBVHIntegrity(int size, BvhExtNodeCompletePort _leaves, BvhIntNodeCompletePort _trunks, int *tag) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		//if (idx >= size) return;
		if (idx == 0)
			if (_leaves.getlca(size) != -1) {
				printf("BVH_SR: \t wrong tail sentinel.\n");
			}
		for (; idx < size; idx += gridDim.x * blockDim.x) {
			int par = _leaves.par(idx), dep = 1;
			bool right = _leaves.mark(idx) & 4;

			if (right) {
				if (_trunks.rc(par) != idx || (_trunks.mark(par) & 2) == 0 || _trunks.rangey(par) != idx || _leaves.lca(idx) & 1 == 0 || _leaves.lca(idx) / 2 != idx)
					printf("BVH_SR: \t leaf %d(as right child) is wrong. type:%d mark: %u par: %d\n", idx, 
						   _trunks.rc(par) != idx | ((_trunks.mark(par) & 2) == 0) << 1 | (_trunks.rangey(par) != idx) << 2, _leaves.mark(idx), par),
						atomicAdd(tag, 1);
			}
			else {
				if (_trunks.lc(par) != idx || (_trunks.mark(par) & 1) == 0 || _trunks.rangex(par) != idx || _leaves.lca(idx) & 1 == 1 || _trunks.rangex(_leaves.lca(idx) / 2) != idx)
					printf("BVH_SR: \t leaf %d(as left child) is wrong. type:%d mark: %u par: %d\n", idx, 
						   _trunks.lc(par) != idx | ((_trunks.mark(par) & 1) == 0) << 1 | (_trunks.rangex(par) != idx) << 2, _leaves.mark(idx), par),
						atomicAdd(tag, 1);
			}
			//if (idx == 171)
			//	printf("BVH_SR: \t %d-th primitive: mark:%o\n", idx, _leaves.mark(idx));
			while (_trunks.par(par) != -1 && *tag < 30) {
				right = _trunks.mark(par) & 4;
				//if (par + _trunks.rangey(par) - _trunks.rangex(par) - 1 != _leaves.par(_trunks.rangey(par)))
				//	printf("BVH_SR: \t \n\nsubtree %d[%d(%d), %d(%d)] is wrong.\n\n", par,
				//		_trunks.rangex(par), _leaves.par(_trunks.rangex(par)), 
				//		_trunks.rangey(par), _leaves.par(_trunks.rangey(par))), atomicAdd(tag, 1);

				if (right) {
					if (_trunks.rc(_trunks.par(par)) != par || (_trunks.mark(_trunks.par(par)) & 2) == 2 || _trunks.rangey(_trunks.par(par)) != _trunks.rangey(par)
						|| (_trunks.mark(_trunks.par(par)) & 1) == 0 && (_trunks.rangex(par) != _trunks.rangey(_trunks.lc(_trunks.par(par))) + 1)
						|| (_trunks.mark(_trunks.par(par)) & 1) == 1 && (_trunks.rangex(_trunks.par(par)) != _trunks.lc(_trunks.par(par))))
						printf("BVH_SR: \t trunk %d(as right child) %d[%d, %d] %d[%d, %d] -> %d[%d, %d].\n", par,
							   _trunks.lc(_trunks.par(par)), _trunks.rangex(_trunks.lc(_trunks.par(par))), _trunks.rangey(_trunks.lc(_trunks.par(par))),
							   par, _trunks.rangex(par), _trunks.rangey(par),
							   _trunks.par(par), _trunks.rangex(_trunks.par(par)), _trunks.rangey(_trunks.par(par))), atomicAdd(tag, 1);
					break;
				}
				//else {
				if (_trunks.lc(_trunks.par(par)) != par || (_trunks.mark(_trunks.par(par)) & 1) == 1 || _trunks.rangex(_trunks.par(par)) != _trunks.rangex(par)
					|| _trunks.par(par) + 1 != par)
					printf("BVH_SR: \t trunk %d(as left child) is wrong.\n", par), atomicAdd(tag, 1);
				//}

				//if (idx == 171)
				//	printf("BVH_SR: \t %d-th primitive: %d level\t %d-th node [%d, %d]\t mark:%o\n", 
				//		idx, dep, par, _trunks.rangex(par), _trunks.rangey(par), _trunks.mark(par));
				++dep;
				par = _trunks.par(par);
			}
			if (dep >= 32) {
				printf("BVH_SR: \t %d-th primitive depth: %d\n", idx, dep);
			}
		}
	}






///==================================================================================================================================================================
/// broadmarkIntegration
///==================================================================================================================================================================


	__global__ void calcMCs_BroadMarkEdition(int size, Aabb* _Aabb, BOX scene, uint* codes) {
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		auto v = _Aabb[idx];
		BOX bv(v);
		const PointType c = bv.center();
		const PointType offset = c - scene._min;
		codes[idx] = morton3D(offset.x / scene.width(), offset.y / scene.height(), offset.z / scene.depth());
	}

	__global__ void buildPrimitives_BroadMarkEdition(int size, BvhPrimitiveCompletePort _prims, int *_primMap, Aabb *_Aabb) {	///< update idx-th _bxs to idx-th leaf
		int idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (idx >= size) return;
		int newIdx = _primMap[idx];
		auto v = _Aabb[idx];
		BOX bv(v);
		_prims.idx(newIdx) = idx;
		_prims.type(newIdx) = static_cast<uint>(ModelType::FixedDeformableType);
		_prims.setBV(newIdx, bv);
	}
}
