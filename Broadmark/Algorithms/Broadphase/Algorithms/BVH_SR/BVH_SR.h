#pragma once

#include "Broadphase\Algorithms\BaseAlgorithm.h"
#include "Broadphase\OverlapChaches\SimpleCache.h"





//Associated BVH_SR 
#include "Frameworks\AppBase\Main.h"
#include "Frameworks\CudaProject\CudaProjectModuleRegister.h"	
#include <utility\CudaHostUtils.h>
#include "setting\BvttFrontSettings.h"
#include "system\Logic\BenchmarkLogic.hpp"

#include "collision\lbvh\LBvh.h"
#include "collision\bvtt_front\BVTTFront.h"

#include "base\Singleton.h"


///==================================================================================================================================================================
/// broadmarkIntegration
///==================================================================================================================================================================


class BVH_SR : public BaseAlgorithm<Object, SimpleCache> {

public:
	BVH_SR();
	virtual ~BVH_SR();
	static bool	initalized;
	void Initialize(InflatedSettings settings, const SceneFrame& frameData) override;

	void UpdateObjects(const SceneFrame& frameData) override;

	void SearchOverlaps() override;

private:
	/// systems (initialized)
	mn::CudaDevice*			TheCudaDevice;
	mn::BenchmarkLogic*		TheLogic;


	mn::LBvhKernelRegister											  _kLBvhKernelRegister;
	mn::BvttFrontKernelRegister								          _kFrontKernelRegister;
	std::unique_ptr<mn::LBvh<mn::ModelType::FixedDeformableType>>	  _bvh;
	std::unique_ptr<mn::BvttFront<mn::BvttFrontType::LooseIntraType>> _fl;




};