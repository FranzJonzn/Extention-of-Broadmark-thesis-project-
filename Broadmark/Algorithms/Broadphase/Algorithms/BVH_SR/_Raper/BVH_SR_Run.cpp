#include "BVH_SR_Run.h"





void BVH_SR_Run::Initialaze(const InflatedSettings& simulationsSettings, const mn::BroadMark_Benchmark& simulationsSettings2) {

	BVH_SR_Run::settings  = simulationsSettings;
	BVH_SR_Run::settings2 = simulationsSettings2;

	BVH_SR_Run* run;
	run				   = new BVH_SR_Run;
	run->TheCudaDevice = mn::CudaDevice::getInstance();
	//run->TheLogic->startup();
//FRANZ: Kommenterat ut för att få mer städad utprintning	
	//printf("BVH_SR \t * Finishing App initialization!\n");


	mn::reportMemory();
//FRANZ: Kommenterat ut för att få mer städad utprintning	
	//printf("BVH_SR \t Begin allocating memory for BVH\n");
	_bvh = std::make_unique<mn::LBvh<mn::ModelType::FixedDeformableType>>(mn::LBvhBuildConfig{});
//FRANZ: Kommenterat ut för att få mer städad utprintning	
	//printf("BVH_SR \t Begin allocating memory for BVTT fronts\n");
	_fl = std::make_unique<mn::BvttFront<mn::BvttFrontType::LooseIntraType>>(mn::BvttFrontIntraBuildConfig<mn::LBvh<mn::ModelType::FixedDeformableType>>(
		_bvh.get(), mn::BvttFrontType::LooseIntraType,
		mn::BvttFrontSettings::ext_front_size(), mn::BvttFrontSettings::int_front_size(),
		mn::BvhSettings::ext_node_size(), mn::BvhSettings::int_node_size()
		));
//FRANZ: Kommenterat ut för att få mer städad utprintning	
	//printf("BVH_SR \t End GPU memory allocations\n");



}
void BVH_SR_Run::RunFrame(const SceneFrame& frameData, const int frameNumber) {
	
	MaintainScheme(frameNumber);
	BVH_SR_Run::_bvh->maintain_BroadMarkEdition(maintainOpts.first, frameData, settings);

}

void BVH_SR_Run::Terminate() {

}

// i stort hämtad från Bullet3GPUAlgorithms då det är den GPU algritmen som fans med från början.
//void BVH_SR_Run::SearchOverlaps(thrust::host_vector<int2> *_pairs) {
//	BVH_SR_Run::_fl->maintain(maintainOpts.second);
//	BVH_SR_Run::_fl->getOverlapingPares(_pairs);
//
//}
int BVH_SR_Run::SearchOverlaps() {
	BVH_SR_Run::_fl->maintain(maintainOpts.second);
	int test  = BVH_SR_Run::_fl->getOverlapingPares();
	printf("i%", test);
	return test;

}

void BVH_SR_Run::MaintainScheme(int frameId)  {
	mn::LBvhFixedDeformableMaintenance bvhOpt;
	mn::BvttFrontLooseIntraMaintenance frontOpt;
	switch (BVH_SR_Run::settings2.schemeOpt.type) {
		case mn::CDSchemeType::GENERATE:
			bvhOpt   = mn::LBvhFixedDeformableMaintenance::BUILD;
			frontOpt = mn::BvttFrontLooseIntraMaintenance::PURE_BVH_CD;
			break;
		case mn::CDSchemeType::FRONT_GENERATE:
			bvhOpt   = mn::LBvhFixedDeformableMaintenance::BUILD;
			frontOpt = mn::BvttFrontLooseIntraMaintenance::GENERATE;
			break;
		case mn::CDSchemeType::STATIC_MANDATORY: ///< bvh cycle should sync with front cycle
			if (mn::CDBenchmarkSettings::enableDivergentMark()) {
				bvhOpt = (frameId - BVH_SR_Run::settings2.stIdx) % mn::BvhSettings::mandatoryRebuildCycle() ? mn::LBvhFixedDeformableMaintenance::REFIT : mn::LBvhFixedDeformableMaintenance::BUILD;
				int frame = (frameId - BVH_SR_Run::settings2.stIdx) % mn::BvttFrontSettings::mandatoryRebuildCycle();
				if (frame == 0)
					frontOpt = frameId == BVH_SR_Run::settings2.stIdx ? mn::BvttFrontLooseIntraMaintenance::GENERATE : mn::BvttFrontLooseIntraMaintenance::UPDATE;
				else if (frame == mn::BvttFrontSettings::mandatoryRebuildCycle() - 1)
					frontOpt = mn::BvttFrontLooseIntraMaintenance::REORDER;
				else
					frontOpt = mn::BvttFrontLooseIntraMaintenance::KEEP;

				if (bvhOpt == mn::LBvhFixedDeformableMaintenance::BUILD)
					frontOpt = mn::BvttFrontLooseIntraMaintenance::GENERATE;
				if ((frameId - BVH_SR_Run::settings2.stIdx + 1) % mn::BvhSettings::mandatoryRebuildCycle() == 0)
					frontOpt = mn::BvttFrontLooseIntraMaintenance::KEEP;
				break;
			}
			else {
				bvhOpt = (frameId - BVH_SR_Run::settings2.stIdx) % mn::BvhSettings::mandatoryRebuildCycle() ? mn::LBvhFixedDeformableMaintenance::REFIT : mn::LBvhFixedDeformableMaintenance::BUILD;
				switch ((frameId - BVH_SR_Run::settings2.stIdx) % mn::BvttFrontSettings::mandatoryRebuildCycle()) {
				case 0: frontOpt = frameId == BVH_SR_Run::settings2.stIdx ? mn::BvttFrontLooseIntraMaintenance::GENERATE : mn::BvttFrontLooseIntraMaintenance::UPDATE; break;
				default: frontOpt = mn::BvttFrontLooseIntraMaintenance::KEEP;
				}
				if (bvhOpt == mn::LBvhFixedDeformableMaintenance::BUILD)
					frontOpt = mn::BvttFrontLooseIntraMaintenance::GENERATE;
				break;
			}
		case mn::CDSchemeType::REFIT_ONLY_FRONT: ///< bvh cycle should sync with front cycle
			if (mn::CDBenchmarkSettings::enableDivergentMark()) {
				bvhOpt = (frameId - BVH_SR_Run::settings2.stIdx) ? mn::LBvhFixedDeformableMaintenance::REFIT : mn::LBvhFixedDeformableMaintenance::BUILD;
				int frame = (frameId - BVH_SR_Run::settings2.stIdx) % mn::BvttFrontSettings::mandatoryRebuildCycle();
				if (frame == 0)
					frontOpt = frameId == BVH_SR_Run::settings2.stIdx ? mn::BvttFrontLooseIntraMaintenance::GENERATE : mn::BvttFrontLooseIntraMaintenance::UPDATE;
				else if (frame == mn::BvttFrontSettings::mandatoryRebuildCycle() - 1)
					frontOpt = mn::BvttFrontLooseIntraMaintenance::REORDER;
				else
					frontOpt = mn::BvttFrontLooseIntraMaintenance::KEEP;
			}
			else {
				bvhOpt = (frameId - BVH_SR_Run::settings2.stIdx) ? mn::LBvhFixedDeformableMaintenance::REFIT : mn::LBvhFixedDeformableMaintenance::BUILD;
				switch ((frameId - BVH_SR_Run::settings2.stIdx) % mn::BvttFrontSettings::mandatoryRebuildCycle()) {
				case 0:  frontOpt = frameId == BVH_SR_Run::settings2.stIdx ? mn::BvttFrontLooseIntraMaintenance::GENERATE : mn::BvttFrontLooseIntraMaintenance::UPDATE; break;
				default: frontOpt = mn::BvttFrontLooseIntraMaintenance::KEEP;
				}
				break;
			}
		default:
			__assume(false);
			//__builtin_unreachable();
	}


	maintainOpts = std::make_pair(bvhOpt, frontOpt);

}


