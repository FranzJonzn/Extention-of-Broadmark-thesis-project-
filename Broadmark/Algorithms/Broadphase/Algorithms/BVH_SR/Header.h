#pragma once


#include "Configs.h"
#include "Macros.h"
#include "Core/Math/Vec3.h"
#include "Core/Settings.h"
#include "Core/Scene.h"
#include "Broadphase/OverlapChaches/SimpleCache.h"
#include "Broadphase/Algorithms/BaseAlgorithm.h"


//Associated BVH_SR 
#include "Frameworks\AppBase\Main.h"
#include "Frameworks\CudaProject\CudaProjectModuleRegister.h"	
#include <utility\CudaHostUtils.h>
#include "setting\BvttFrontSettings.h"
#include "system\Logic\BenchmarkLogic.hpp"

#include "collision\lbvh\LBvh.h"
#include "collision\bvtt_front\BVTTFront.h"

#include "base\Singleton.h"

class Header : public BaseAlgorithm<Object, SimpleCache> {


public:
	Header();
	virtual ~Header() { std::cout << "Header_Test_Av_algo: ~Header() \n"; }


	void Initialize(InflatedSettings settings, const SceneFrame& frameData) override {
		std::cout << "Header_Test_Av_algo_: Initialize(InflatedSettings settings, const SceneFrame& frameData)\n";
	}

	void UpdateObjects(const SceneFrame& frameData) override {
		std::cout << "Header_Test_Av_algo_: UpdateObjects(const SceneFrame& frameData)\n";
	}

	void SearchOverlaps() override {
		std::cout << "Header_Test_Av_algo_: SearchOverlaps()\n";
	}



private:
	/// systems (initialized)
	//mn::CudaDevice*			TheCudaDevice;
	//mn::BenchmarkLogic*		TheLogic;


	mn::LBvhKernelRegister											  _kLBvhKernelRegister;
	//mn::BvttFrontKernelRegister								          _kFrontKernelRegister;
	//std::unique_ptr<mn::LBvh<mn::ModelType::FixedDeformableType>>	  _bvh;
	//std::unique_ptr<mn::BvttFront<mn::BvttFrontType::LooseIntraType>> _fl;





};


