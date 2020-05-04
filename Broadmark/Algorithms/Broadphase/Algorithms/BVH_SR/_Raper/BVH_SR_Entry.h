#pragma once

#include "Broadphase\Algorithms\BaseAlgorithm.h"
#include "Broadphase\OverlapChaches\SimpleCache.h"




//Associated BVH_SR 
#include "Frameworks\AppBase\Main.h"
#include "Frameworks\CudaProject\CudaProjectModuleRegister.h"	

#include"BVH_SR_Run.h"



class BVH_SR_Entry : public BaseAlgorithm<Object, SimpleCache> 
{
public:
	void Initialize(InflatedSettings settings, const SceneFrame& frameData) override;
	void UpdateObjects(const SceneFrame& frameData) override;
	void UpdateStructures() override;
	void SearchOverlaps() override;

private:
	BVH_SR_Run Bvh;
};