#include "BVH_SR.h"
#include <fstream>

///==================================================================================================================================================================
/// broadmarkIntegration
///==================================================================================================================================================================



bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();

bool BVH_SR::initalized = false;



//BVH_SR::BVH_SR(){
//	std::cout << "fsdfsdfaaaaaaaadddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd\n";
//	if (!initalized) {
//
//		std::cout << "BVH_SR_S";
//		mn::Main::Initialize();
//		BVH_SR::TheCudaDevice = mn::CudaDevice::getInstance();
//		BVH_SR::TheLogic      = mn::BenchmarkLogic::getInstance();
//
//	
//
//		initalized = true;
//		std::cout << "BVH_SR_E";
//	}
//}
BVH_SR::~BVH_SR(){


	mn::Main::Terminate();
}

void BVH_SR::Initialize(InflatedSettings settings, const SceneFrame& frameData){
	std::cout << "hej";
	if (!initalized) { return; }
	std::cout << "Initialize";

	BaseAlgorithm<Object, SimpleCache>::Initialize(settings, frameData);

	mn::reportMemory();
	printf("Begin allocating memory for BVH\n");
	_bvh = std::make_unique<mn::LBvh<mn::ModelType::FixedDeformableType>>(mn::LBvhBuildConfig{});
	printf("Begin allocating memory for BVTT fronts\n");
	_fl = std::make_unique<mn::BvttFront<mn::BvttFrontType::LooseIntraType>>(mn::BvttFrontIntraBuildConfig<mn::LBvh<mn::ModelType::FixedDeformableType>>(
		_bvh.get(), mn::BvttFrontType::LooseIntraType,
		mn::BvttFrontSettings::ext_front_size(), mn::BvttFrontSettings::int_front_size(),
		mn::BvhSettings::ext_node_size(), mn::BvhSettings::int_node_size()
		));
	printf("End GPU memory allocations\n");

}

void BVH_SR::UpdateObjects(const SceneFrame& frameData) {
	if (!initalized) { return; }
	std::cout << "UpdateObjects";
	auto maintainOpts = BVH_SR::TheLogic->maintainScheme();

	BVH_SR::_bvh->maintain_BroadMarkEdition(maintainOpts.first, frameData, m_settings);
	BVH_SR::_fl->maintain(maintainOpts.second);

	

}




void BVH_SR::SearchOverlaps(){
	std::cout << "SearchOverlapssdvdxvdxvsxvsfadvasvsdvsadvsd";
	if (!initalized) { return; }
	std::cout << "SearchOverlaps";
}
