#include "BVH_SR_Entry.h"


//bool mn::ModuleRegister::s_bInitializeRegistered = mn::ModuleRegister::RegisterInitialize();
//bool mn::ModuleRegister::s_bTerminateRegistered  = mn::ModuleRegister::RegisterTerminate();



void BVH_SR_Entry::Initialize(InflatedSettings settings, const SceneFrame& frameData)
{


	BVH_SR_Entry::frameNumber = 1;
	BaseAlgorithm<Object, NullCache>::Initialize(settings, frameData);


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

	m_cache.m_numOfOverlaps = Bvh.SearchOverlaps();

//#pragma warning(suppress : 4996)
//	thrust::host_vector<int2>	m_pairs;
//#pragma warning(disable : 4996)
	//Bvh.SearchOverlaps(&m_pairs);
	/*int lenght = m_pairs.size();

	for (size_t i = 0; i < lenght; i++) 
		m_cache.AddPair(&m_objects[m_pairs[i].x], &m_objects[m_pairs[i].y]);
*/
	//m_pairs.clear();
}
