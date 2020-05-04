#include "BVH_SR_Entry.h"


//bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
//bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();



void BVH_SR_Entry::Initialize(InflatedSettings settings, const SceneFrame& frameData)
{


	BVH_SR_Entry::frameNumber = 1;
	BaseAlgorithm<Object, SimpleCache>::Initialize(settings, frameData);




	mn::BroadMark_Benchmark schemeSettings;
	schemeSettings.stIdx     = 1;
	schemeSettings.schemeOpt = getSceme();



	Bvh.Initialaze(settings, schemeSettings);
}
void BVH_SR_Entry::UpdateObjects(const SceneFrame& frameData) {
	BVH_SR_Entry::frameNumber = BVH_SR_Entry::frameNumber + 1;
	Bvh.RunFrame(frameData, BVH_SR_Entry::frameNumber);
}

void BVH_SR_Entry::UpdateStructures() {

}

void BVH_SR_Entry::SearchOverlaps(){
	std::cout << "SearchOverlaps\n";
}
