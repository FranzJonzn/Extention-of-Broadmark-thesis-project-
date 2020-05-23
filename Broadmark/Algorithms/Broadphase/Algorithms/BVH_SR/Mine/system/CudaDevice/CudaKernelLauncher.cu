#ifndef __CUDA_LAUNCHER_H_
#define __CUDA_LAUNCHER_H_

#include "utility\CudaExecutionPolicy.h"
#include "CudaDevice.h"
#include "system\Log\Logger.hpp"

namespace mn {

	template <typename... Arguments>
	void debugLaunch(int gs, int bs, void(*f)(Arguments...), Arguments... args) {
		cudaError_t error;
		f << <gs, bs >> >(args...);
		cudaDeviceSynchronize();
		error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR1: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}

	template <typename... Arguments>
	void recordLaunch(std::string tag, int gs, int bs, size_t mem, void(*f)(Arguments...), Arguments... args) {
		if (!mem) {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs >> > (args...);
			Logger::tock<TimerType::GPU>(tag);
		} else {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs, mem >> > (args...);
			Logger::tock<TimerType::GPU>(tag);
		}
		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR2: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}

	template <typename... Arguments>
	void recordLaunch(const char* tag, int gs, int bs, size_t mem, void(*f)(Arguments...), Arguments... args) {
		if (!mem) {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs >> > (args...);
			Logger::tock<TimerType::GPU>(tag);
		} else {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs, mem >> > (args...);
			Logger::tock<TimerType::GPU>(tag);
		}
		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR3: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}

	template <typename... Arguments>
	void cleanLaunch(int gs, int bs, void(*f)(Arguments...), Arguments... args) {
		f << <gs, bs >> >(args...);
	}

	template <typename... Arguments>
	void configuredLaunch(LaunchInput instr, void(*f)(Arguments...), Arguments... args)
	
	{
	
		ExecutionPolicy p = CudaDevice::getInstance()->launchConfig(instr.name(), instr.threads());

		if (p.getSharedMemBytes() == 0) {

			Logger::tick<TimerType::GPU>();
			f << <p.getGridSize(), p.getBlockSize() >> >(args...);
			Logger::tock<TimerType::GPU>(instr.name());

		}
		else {

			Logger::tick<TimerType::GPU>();
			f << <p.getGridSize(), p.getBlockSize(), p.getSharedMemBytes() >> >(args...);
			Logger::tock<TimerType::GPU>(instr.name());
		}

		cudaError_t error = cudaGetLastError();

		if (error != cudaSuccess) printf("BVH_SR4: \t Kernel launch failure %s [%s]\n", cudaGetErrorString(error),instr.name());
	}

	/*
	template <typename... Arguments>
	void debugLaunch(int gs, int bs, void(*f)(Arguments...), Arguments&&... args) {
		cudaError_t error;
		f << <gs, bs >> >(std::forward<Arguments>(args)...);
		cudaDeviceSynchronize();
		error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}

	template <typename... Arguments>
	void recordLaunch(std::string&& tag, int gs, int bs, size_t mem, void(*f)(Arguments...), Arguments&&... args) {
		if (!mem) {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs >> > (std::forward<Arguments>(args)...);
			Logger::tock<TimerType::GPU>(tag);
		}
		else {
			Logger::tick<TimerType::GPU>();
			f << <gs, bs, mem >> > (std::forward<Arguments>(args)...);
			Logger::tock<TimerType::GPU>(tag);
		}
		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}

	template <typename... Arguments>
	void cleanLaunch(int gs, int bs, void(*f)(Arguments...), Arguments&&... args) {
		f << <gs, bs >> >(std::forward<Arguments...>(std::forward<Arguments>(args)...));
	}

	template <typename... Arguments>
	void configuredLaunch(LaunchInput&& instr, void(*f)(Arguments...), Arguments&&... args) {
		ExecutionPolicy p = CudaDevice::getInstance()->launchConfig(instr.name(), instr.threads());
		if (p.getSharedMemBytes() == 0) {
			Logger::tick<TimerType::GPU>();
			f << <p.getGridSize(), p.getBlockSize() >> >(std::forward<Arguments>(args)...);
			Logger::tock<TimerType::GPU>(instr.name());
		}
		else {
			Logger::tick<TimerType::GPU>();
			f << <p.getGridSize(), p.getBlockSize(), p.getSharedMemBytes() >> >(std::forward<Arguments>(args)...);
			Logger::tock<TimerType::GPU>(instr.name());
		}
		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) printf("BVH_SR: \t Kernel launch failure %s\n", cudaGetErrorString(error));
	}
	*/
}

#endif