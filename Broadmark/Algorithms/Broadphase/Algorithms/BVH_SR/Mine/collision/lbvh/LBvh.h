/**	\file	LBvh.h
*	\brief	LBvh selector & bvh kernel register
*	\author	littlemine
*	\ref	article - Efficient BVH-based collision detection scheme for deformable models with ordering and restructuring
*/

#ifndef __LBVH_H_
#define __LBVH_H_

#include "setting\BvhSettings.h"
#include "LBvhFixedDeformable.h"
#include "system\CudaDevice\CudaDevice.h"
#include "utility\CudaKernelUtils.cuh"

namespace mn {
	
	template<ModelType modelType>
	struct LBvhSelector;

	template<>
	struct LBvhSelector<ModelType::FixedDeformableType> {
		using type = LBvhFixedDeformable;
	};


	template<ModelType modelType>
	using LBvh = typename LBvhSelector<modelType>::type;

	class LBvhKernelRegister {
	public:
		LBvhKernelRegister() {
			CudaDevice* device = CudaDevice::getInstance();
			device->startup();								// <-BroadMarkIntegration (fickdet inte att fungera annars);

			/// components
			device->registerKernel("GatherBVs"                   , gatherBVs                   , cudaFuncCachePreferL1, false);
			device->registerKernel("ScatterBVs"                  , scatterBVs                  , cudaFuncCachePreferL1, false);
			device->registerKernel("GatherPrims"                 , gatherPrims                 , cudaFuncCachePreferL1, false);
			device->registerKernel("ScatterPrims"                , scatterPrims                , cudaFuncCachePreferL1, false);
			device->registerKernel("MarkPrimSplitPos"            , markPrimSplitPos            , cudaFuncCachePreferL1, false);
			device->registerKernel("CollapsePrimitives"          , collapsePrimitives          , cudaFuncCachePreferL1, false);
			device->registerKernel("CalcExtNodeSplitMetrics"     , calcExtNodeSplitMetrics     , cudaFuncCachePreferL1, false);
			device->registerKernel("CalcExtNodeRestrSplitMetrics", calcExtNodeRestrSplitMetrics, cudaFuncCachePreferL1, false);
			device->registerKernel("ScatterIntNodes"             , scatterIntNodes             , cudaFuncCachePreferL1, false);

			device->registerKernel("CalcMC64s", calcMC64s, cudaFuncCachePreferL1, false);
			/// build
			device->registerKernel("CalcPrimMap"          , calcInverseMapping              , cudaFuncCachePreferL1, false);			
			device->registerKernel("BuildIntNodes"        , buildIntNodes                   , cudaFuncCachePreferL1, false);
			device->registerKernel("CalcIntNodeOrders"    , calcIntNodeOrders               , cudaFuncCachePreferL1, false);
			device->registerKernel("UpdateBvhExtNodeLinks", updateBvhExtNodeLinks           , cudaFuncCachePreferL1, false);
			device->registerKernel("ReorderIntNode"       , reorderIntNode                  , cudaFuncCachePreferL1, false);
			/// refit
			device->registerKernel("RefitExtNode" , refitExtNode , cudaFuncCachePreferL1, false);
			device->registerKernel("RefitIntNode" , refitIntNode , cudaFuncCachePreferL1, false);
			device->registerKernel("UpdateIntNode", updateIntNode, cudaFuncCachePreferL1, true);
			/// restructure
			device->registerKernel("CalibrateLeafRangeMarks", calibrateLeafRangeMarks, cudaFuncCachePreferL1, false);
			device->registerKernel("CalibrateRestrRoots"	, calibrateRestrRoots    , cudaFuncCachePreferL1, false);
			device->registerKernel("SelectPrimitives"		, selectPrimitives       , cudaFuncCachePreferL1, false);
			device->registerKernel("UpdatePrimMap"			, updatePrimMap          , cudaFuncCachePreferL1, false);
			device->registerKernel("RestrIntNodes"			, restrIntNodes          , cudaFuncCachePreferL1, false);
			device->registerKernel("CalcRestrIntNodeOrders"	, calcRestrIntNodeOrders , cudaFuncCachePreferL1, false);
			device->registerKernel("ReorderRestrIntNodes"	, reorderRestrIntNodes   , cudaFuncCachePreferL1, false);


///==================================================================================================================================================================
// FJ_BME:
///==================================================================================================================================================================
		

			/// morton codes
			device->registerKernel("CalcMCs_BME", calcMCs_BroadMarkEdition, cudaFuncCachePreferL1, false);

			/// build
			device->registerKernel("BuildPrims_BME", buildPrimitives_BroadMarkEdition, cudaFuncCachePreferL1, false); 

		    /// refit
			device->registerKernel("RefitExtNode_BME", refitExtNode_BroadMarkEdition, cudaFuncCachePreferL1, false);

			/// restructure
			device->registerKernel("CalcRestrMCs_BME"        , calcRestrMCs_BroadMarkEdition        , cudaFuncCachePreferL1, false);
			device->registerKernel("UpdatePrimAndExtNode_BME", updatePrimAndExtNode_BroadMarkEdition, cudaFuncCachePreferL1, false);
	
		}
		~LBvhKernelRegister() {}
	};
}

#endif