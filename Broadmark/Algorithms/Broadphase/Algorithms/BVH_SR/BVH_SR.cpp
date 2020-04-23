#include "BVH_SR.h"




bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();

BVH_SR::BVH_SR(){






	mn::Main::Initialize();
	BVH_SR::TheCudaDevice	= mn::CudaDevice::getInstance();
	BVH_SR::TheLogic		= mn::BenchmarkLogic::getInstance();


	mn::reportMemory();
	printf("Begin allocating memory for BVH\n");
	BVH_SR::_bvh = std::make_unique<mn::LBvh<mn::ModelType::RigidType>>(mn::LBvhBuildConfig{});
	printf("Begin allocating memory for BVTT fronts\n");
	BVH_SR::_fl = std::make_unique<mn::BvttFront<mn::BvttFrontType::LooseIntraType>>(mn::BvttFrontIntraBuildConfig<mn::LBvh<mn::ModelType::RigidType>>(
		_bvh.get(), mn::BvttFrontType::LooseIntraType,
		mn::BvttFrontSettings::ext_front_size(), mn::BvttFrontSettings::int_front_size(),
		mn::BvhSettings::ext_node_size(), mn::BvhSettings::int_node_size()
		));
	printf("End GPU memory allocations\n");

	initalized				= true;

}
BVH_SR::~BVH_SR(){


	mn::Main::Terminate();
}

void BVH_SR::Initialize(InflatedSettings settings, const SceneFrame& frameData){
	if (!initalized) { return; }
}

void BVH_SR::UpdateObjects(const SceneFrame& frameData) {
	if (!initalized) { return; }

	auto maintainOpts = BVH_SR::TheLogic->b_maintainScheme();

	BVH_SR::_bvh->maintain_BroadMarkEdition(maintainOpts.first, frameData, m_settings);
	BVH_SR::_fl->maintain(maintainOpts.second);


}




void BVH_SR::SearchOverlaps(){
	if (!initalized) { return; }
}
