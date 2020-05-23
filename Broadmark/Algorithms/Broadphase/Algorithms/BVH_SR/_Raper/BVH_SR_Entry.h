#pragma once

#include "Broadphase\Algorithms\BaseAlgorithm.h"
#include "Broadphase\OverlapChaches\SimpleCache.h"


#include "Broadphase/OverlapChaches/NullCache.h"

//Associated BVH_SR 
//#include "Frameworks\AppBase\Main.h"
#include "Frameworks\CudaProject\CudaProjectModuleRegister.h"	

#include"BVH_SR_Run.h"

#include "system\CudaDevice\CudaDevice.h"
#include "system\Log\Logger.hpp"


#include <thrust\host_vector.h>

	



class BVH_SR_Entry : public BaseAlgorithm<Object, NullCache>
{
public:




	 ~BVH_SR_Entry();
	void Initialize(InflatedSettings settings, const SceneFrame& frameData) override;
	void UpdateObjects(const SceneFrame& frameData) override;
	void UpdateStructures() override;
	void SearchOverlaps() override;


protected:
	virtual mn::Scheme getSceme() = 0;


private:
	BVH_SR_Run Bvh;
	int frameNumber;
};


class BVH_SR_STATIC_MANDATORY : public BVH_SR_Entry
{
	virtual mn::Scheme getSceme() override 
	{
		return { mn::CDSchemeType::STATIC_MANDATORY, "static_mandatory" };
	}

};
class BVH_SR_REFIT_ONLY_FRONT : public BVH_SR_Entry
{
	virtual mn::Scheme getSceme() override
	{
		return { mn::CDSchemeType::REFIT_ONLY_FRONT, "refit_only" };
	}

};
class BVH_SR_GENERATE : public BVH_SR_Entry
{
	virtual mn::Scheme getSceme() override
	{
		return { mn::CDSchemeType::GENERATE, "generate" };
	}

};
class BVH_SR_FRONT_GENERATE : public BVH_SR_Entry
{
	virtual mn::Scheme getSceme() override
	{
		return { mn::CDSchemeType::FRONT_GENERATE, "front_generate" };
	}

};