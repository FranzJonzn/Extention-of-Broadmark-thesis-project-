#include "BVH_SR_Entry.h"


bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();



void BVH_SR_Entry::Initialize(InflatedSettings settings, const SceneFrame& frameData)
{
	BaseAlgorithm<Object, SimpleCache>::Initialize(settings, frameData);




	mn::Main::Initialize();


	Bvh.Initialaze();
}
void BVH_SR_Entry::UpdateObjects(const SceneFrame& frameData) {

}

void BVH_SR_Entry::UpdateStructures() {

}

void BVH_SR_Entry::SearchOverlaps(){

}
