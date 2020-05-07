#include "BVH_SR_Entry.h"


//bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
//bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();



void BVH_SR_Entry::Initialize(InflatedSettings settings, const SceneFrame& frameData)
{


	BVH_SR_Entry::frameNumber = 1;
	BaseAlgorithm<Object, SimpleCache>::Initialize(settings, frameData);


	mn::Logger::getInstance()->startup();

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
	
	Bvh.SearchOverlaps(&m_pairs);

	for (size_t i = 0; i < m_pairs.size(); i++) {
		Object *p1, *p2;
		int2 _cp = m_pairs[i];

		if (_cp.x < _cp.y) {
			p1 = &m_objects[_cp.x];
			p2 = &m_objects[_cp.y];
		}
		else {
			p1 = &m_objects[_cp.y];
			p2 = &m_objects[_cp.x];

		}

		if (p1 != p2) {
			m_cache.AddPair(p1, p2);
		}
	}
	m_pairs.clear();
}
