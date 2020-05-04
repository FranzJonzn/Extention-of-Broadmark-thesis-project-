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


class BVH_SR_Run {

public:


	void Initialaze();
	void RunFrame(const SceneFrame& frameData);

	void Terminate();

	void SearchOverlaps();

private:
	/// systems (initialized)
	mn::CudaDevice*		TheCudaDevice;
	mn::BenchmarkLogic* TheLogic;

	// for CD benchmark testing
	mn::LBvhKernelRegister												_kLBvhKernelRegister;
	mn::BvttFrontKernelRegister											_kFrontKernelRegister;
	std::unique_ptr<mn::LBvh<mn::ModelType::FixedDeformableType>>		_bvh;
	std::unique_ptr<mn::BvttFront<mn::BvttFrontType::LooseIntraType>>	_fl;

}; 