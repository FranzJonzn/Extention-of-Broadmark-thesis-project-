#pragma once
#include "Broadphase\Algorithms\BaseAlgorithm.h"
#include "Broadphase\OverlapChaches\SimpleCache.h"


#include <string>
#include "base\Singleton.h"

// framework modules
#include "world\Scene.h"
#include "collision\lbvh\LBvh.h"
#include "collision\bvtt_front\BVTTFront.h"
#include "system\Logic\BenchmarkLogic.hpp"

#include "Frameworks\CudaProject\CudaProjectModuleRegister.h"	
#include <utility\CudaHostUtils.h>
#include "setting\BvttFrontSettings.h"


class BVH_SR_Run {

public:


	void Initialaze(const InflatedSettings& simulationsSettings, const mn::BroadMark_Benchmark& simulationsSettings2);
	
	void RunFrame(const SceneFrame& frameData, const int frameNumber);

	void Terminate();

	void SearchOverlaps();

private:


	std::pair<mn::LBvhFixedDeformableMaintenance, mn::BvttFrontLooseIntraMaintenance> MaintainScheme(int frameId) const;








	InflatedSettings        settings;
	mn::BroadMark_Benchmark    settings2; // replacement for the seting that was in BVH_SR original system

	/// systems (initialized)
	mn::CudaDevice*		TheCudaDevice;
	mn::BenchmarkLogic* TheLogic;

	// for CD benchmark testing
	mn::LBvhKernelRegister												_kLBvhKernelRegister;
	mn::BvttFrontKernelRegister											_kFrontKernelRegister;
	std::unique_ptr<mn::LBvh<mn::ModelType::FixedDeformableType>>		_bvh;
	std::unique_ptr<mn::BvttFront<mn::BvttFrontType::LooseIntraType>>	_fl;

}; 