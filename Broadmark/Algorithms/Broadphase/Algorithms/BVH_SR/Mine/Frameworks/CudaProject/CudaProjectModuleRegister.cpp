#include "CudaProjectModuleRegister.h"
#include "Frameworks\AppBase\Main.h"


#include "system\CudaDevice\CudaDevice.h"
#include "system\Log\Logger.hpp"

#include "CudaProjectApp.h"

namespace mn {

	bool ModuleRegister::RegisterInitialize() {
		Main::AddInitializer(CudaDevice::startup);
		Main::AddInitializer(Logger::startup);

		return true;
	}

	bool ModuleRegister::RegisterTerminate() {
		Main::AddTerminator(Logger::shutdown);
		Main::AddTerminator(CudaDevice::shutdown);
		return false;
	}

}
