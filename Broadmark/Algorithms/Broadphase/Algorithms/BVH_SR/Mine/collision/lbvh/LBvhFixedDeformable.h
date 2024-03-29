#ifndef __LBVH_FIXEDDEFORMABLE_H_
#define __LBVH_FIXEDDEFORMABLE_H_

#include <thrust\device_vector.h>

#include "setting\BvhSettings.h"
#include "Core\Scene.h"	  //broadmarkIntegration
#include "Core\Settings.h"//broadmarkIntegration


#include "BvhExtNode.h"
#include "BvhIntNode.h"
#include "LBvhKernels.cuh"
#include "collision\Auxiliary\BvhRestrLog.h"
#include "utility\CudaThrustUtils.hpp"

namespace mn {
	class FlOrderCompletePort;

	class CudaDevice;

	/// for now, maintenance work is configured by this input alone, regardless of Bvh state
	enum class LBvhFixedDeformableMaintenance
	{ BUILD, REFIT, UPDATE };

	/*
	 * \brief	LBvhFixedDeformable
	 * \note	RAII
	 */
	class LBvhFixedDeformable {
	public:
		LBvhFixedDeformable() = delete;
		LBvhFixedDeformable(LBvhBuildConfig&& config);
		~LBvhFixedDeformable();
		void	maintain_BroadMarkEdition(LBvhFixedDeformableMaintenance scheme, const SceneFrame& fdata, const InflatedSettings& settings); // broadmarkIntegration

		BvhPrimitiveArray&	cprim()				   { return cbvh().lvs().getPrimitiveArray(); }
		BvhExtNodeArray&	clvs()				   { return cbvh().lvs();					  }
		BvhIntNodeArray&	ctks()				   { return cbvh().tks();					  }
		int*				getPrevLbds()		   { return getRawPtr(d_prevLbds);			  }
		int*				getPrevRbds()		   { return getRawPtr(d_prevRbds);			  }
		BvhRestrLog&		restrLog()			   { return _restrLog;						  }
		bool				logUpdated()     const { return _restrLog.getUpdateTag();		  }
		int 				bvhOptTag()		 const { return _restrLog.getBvhOptTag();		  }
		uint				getExtNodeSize()       { return cbvh().extSize();				  }
		uint				getIntNodeSize()       { return cbvh().intSize();				  }
		uint				getPrimNodeSize()      { return cbvh().primSize();				  }
		int*				getPrimMap()           { return getRawPtr(d_primMap);			  }
		const Aabb*			getAabbs()		 const { return d_aabb;							  }//FJ_BME:

	private:
		class	Bvh {
		public:
			Bvh() {}
			~Bvh() {}
			void setup(LBvhBuildConfig config) {
				_lvs.setup(config.primSize, config.extSize);
				_tks.setup(config.intSize);
				checkCudaErrors(cudaMalloc((void**)&_bv, sizeof(BOX)));
			}
			void cleanup() {
				_lvs.cleanup();
				_tks.cleanup();
				checkCudaErrors(cudaFree(_bv));
			}
			int		&primSize() { return _primSize; }
			int		&extSize() { return _extSize; }
			int		&intSize() { return _intSize; }
			BOX*	&bv() { return _bv; }
			BvhExtNodeArray	&lvs() { return _lvs; }
			BvhIntNodeArray	&tks() { return _tks; }
		private:
			int				_primSize, _extSize, _intSize;
			BvhExtNodeArray	_lvs;
			BvhIntNodeArray	_tks;
			BOX*			_bv;
		};

		CudaDevice*	TheCudaDevice;
		//Bvh&	cbvh() { return _bvhs.cho(); }
		Bvh&	cbvh() { return _bvh; }

// FJ_BME:
		/// main maintenance methods
		void	build_BroadMarkEdition(const Aabb& worldAabb);
		void	refit_BroadMarkEdition();
		void	update_BroadMarkEdition();///< gather degradation infos 
		/// 
		bool	restructure_BroadMarkEdition(const Aabb& worldAabb);
		void	updatePrimData_BroadMarkEdition(const SceneFrame& fdata, const InflatedSettings& settings);
	
		
		
		void	reorderPrims();
		void	reorderIntNodes();
		///
		void	checkPrimitiveMap();
		void	checkBvhValidity();


		Aabb*								d_aabb;	// FJ_BME:

		/// bvh
		Bvh									_bvh;
		/// auxiliary structures during maintenance procedure
		BvhPrimitiveArray					_unsortedPrims;
		BvhIntNodeArray						_unsortedTks;
		thrust::device_vector<int>			d_primMap;				///< map from primitives to leaves
		thrust::device_vector<int>			d_tkMap;
		thrust::device_vector<uint>			d_offsetTable;
		// sort
		thrust::device_vector<uint>			d_count;
		thrust::device_vector<uint>			d_keys32;
		thrust::device_vector<uint64>		d_keys64;
		thrust::device_vector<int>			d_vals;
		/// single values
		int									_numTaskedPrims, _numRtSubtree, _numRtIntNode, _numRtPrim, _numRtExtNode;
		int									*d_numRtSubtree, *d_numRtIntNode;
		// restructure
		BvhRestrLog							_restrLog;
		thrust::device_vector<int>			d_prevLbds, d_prevRbds;
		thrust::device_vector<int>			d_gatherMap;
		thrust::device_vector<int>			d_taskSequence;
		thrust::device_vector<int>			d_sequence;
		thrust::device_vector<int>			d_rtSubtrees;			///< stores root nodes of subtrees

		thrust::device_vector<uint>			d_sizePerSubtree;
		thrust::device_vector<int>			d_begPerSubtree;

	};

}

#endif