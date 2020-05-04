#pragma once

#include<string>

namespace mn {

	class CustomPrint{

	static void CustomPrintf(std::string text) 
	{
		std::string t2 = "BVH_SR: \t " + text;
		printf(t2.c_str());
	}

	};
}