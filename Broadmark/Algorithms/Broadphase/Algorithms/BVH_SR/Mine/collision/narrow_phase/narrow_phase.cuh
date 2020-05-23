#ifndef __NARROW_PHASE_CUH_
#define __NARROW_PHASE_CUH_

#include "collision/lbvh/BvhPrimitiveNode.h"
#include "Core\Math\Aabb.h"
namespace mn {

	using uint = unsigned int;

//	__global__ void simpleNarrowPhase(uint numpair, int2* _cps, const int3* _indices, const PointType* _vertices, int* _actualCpNum);
		//FJ_BM:
	__global__ void simpleNarrowPhase_BME(uint numpair, int2* _cps, const Aabb* _aabb, int* _actualCpNum);
}

#endif