#include "BVH_SR_Run.h"





void BVH_SR_Run::Initialaze() {
	BVH_SR_Run* run;
	run				   = new BVH_SR_Run;
	run->TheCudaDevice = mn::CudaDevice::getInstance();
	run->TheLogic      = mn::BenchmarkLogic::getInstance();

}
void BVH_SR_Run::RunFrame(const SceneFrame& frameData) {

}

void BVH_SR_Run::Terminate() {

}

void BVH_SR_Run::SearchOverlaps() {

}