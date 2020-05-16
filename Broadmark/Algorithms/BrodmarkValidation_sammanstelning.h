#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <chrono>///////////////////////////////////////////////////////////////////


class BrodmarkValidation_sammanstelning {
	
public:
	
	void CompileResult()
	{
		int fileCount              = 0;
		int frameCount             = 0;
		long double averageMyN     = 0.0;
		long double averageThereN  = 0.0;
		long double averageDifN    = 0.0;
		std::string text           = "";

		std::vector<double> allMyN{};
		std::vector<double> allThereN{};

		double myN_GLOBAL_MIN    = 100000000000.0;
		double thereN_GLOBAL_MIN = 100000000000.0;
		double myN_GLOBAL_MAX    = -1.0;
		double thereN_GLOBAL_MAX = -1.0;


		bool rune = true;
		while (rune)
		{
			++fileCount;

			std::string file = path + fileName + std::to_string(fileCount) + fileType;

			std::ifstream ifile(file, std::ios::in);


			if (!ifile.is_open()) { rune = false; }
			else {
				std::cout << file << std::endl;

				std::string str = "";
				int simuleringBildCount = 1;

				text += "\nSimulering " + std::to_string(fileCount) + " =========================================================================================:\n";
				double  myN_LOCAL_MIN = 100000000000.0;
				double  thereN_LOCAL_MIN = 100000000000.0;
				double  myN_LOCAL_MAX = -1.0;
				double  thereN_LOCAL_MAX = -1.0;

				while (std::getline(ifile, str)) {

					std::stringstream ss(str);


					text += "Bild " + std::to_string(simuleringBildCount) + ": " + str + "\n";

					double myN, thereN, difN;

					ss.ignore(256, ':');
					ss >> myN;
					ss.ignore(256, ':');
					ss >> thereN;
					difN = (myN / thereN) - 1;


					allMyN.push_back(myN);
					allThereN.push_back(thereN);


					if (thereN_LOCAL_MAX < thereN) { thereN_LOCAL_MAX = thereN; }
					if (myN_LOCAL_MAX    < myN   ) { myN_LOCAL_MAX    = myN; }
					if (thereN_LOCAL_MIN > thereN) { thereN_LOCAL_MIN = thereN; }
					if (myN_LOCAL_MIN    > myN   ) { myN_LOCAL_MIN    = myN; }


					averageMyN    += myN;
					averageThereN += thereN;
					averageDifN   += difN;
					++simuleringBildCount;
					++frameCount;
				}

				if (thereN_GLOBAL_MAX < thereN_LOCAL_MAX) { thereN_GLOBAL_MAX = thereN_LOCAL_MAX; }
				if (myN_GLOBAL_MAX    < myN_LOCAL_MAX   ) { myN_GLOBAL_MAX    = myN_LOCAL_MAX;    }
				if (thereN_GLOBAL_MIN > thereN_LOCAL_MIN) { thereN_GLOBAL_MIN = thereN_LOCAL_MIN; }
				if (myN_GLOBAL_MIN    > myN_LOCAL_MIN   ) { myN_GLOBAL_MIN    = myN_LOCAL_MIN;    }

				ifile.close();

			}
		}



		averageMyN    = averageMyN / (double)frameCount;
		averageThereN = averageThereN / (double)frameCount;
		averageDifN   = averageDifN / (double)frameCount;


		auto myLamda = std::for_each(   
										allMyN.begin(),
		                                allMyN.end(),
		                                [mean = averageMyN, variansen = 0.0](auto value) mutable
		                                {
		                                	if constexpr (std::is_same_v<decltype(value), const char*>) { return variansen; }
		                                	else { return 	variansen += (value - mean)*(value - mean); }
		                                }
		                            );


		auto thereLamda = std::for_each(
			                                allThereN.begin(),
			                                allThereN.end(),
			                                [mean = averageThereN, variansen = 0.0](auto value) mutable
		                                    {
		                                    	if constexpr (std::is_same_v<decltype(value), const char*>) { return variansen; }
		                                    	else { return 	variansen += (value - mean)*(value - mean); }
		                                    }
		                               );


		double myVarians    = (double)myLamda   ("variansen") / (double)frameCount;
		double thereVarians = (double)thereLamda("variansen") / (double)frameCount;
		double myStd        = std::sqrt(myVarians);
		double therestd     = std::sqrt(thereVarians);



		std::string result;
		result = "RESULTAT AV VALIDERING program av Franz Jonzon 2020-03-17\n";
		result += "============================================================================================== \n";
		result += "Antalet lästa filer: \t\t\t\t\t"                       + std::to_string(fileCount - 1)                                                 + "\t stycken\n";
		result += "Antalet bilder: \t\t\t\t\t"                            + std::to_string(frameCount)                                                    + "\t stycken\n";
		result += "Medelvärde för sekunderna jag mätte: \t\t\t"           + std::to_string(averageMyN)                                                    + " sekunder\n";
		result += "\tStandardavvikelse jag mätte: \t\t\t"                 + std::to_string(myStd)                                                         + " sekunder\n";
		result += "\tVarianesnen jag mätte: \t\t\t\t"                     + std::to_string(myVarians)                                                     + " sekunder\n";
		result += "\tmax/min jag uppmätt: \t\t\t\t"                       + std::to_string(myN_GLOBAL_MAX)   + " / " + std::to_string(myN_GLOBAL_MIN)     + " sekunder\n";
																	      
		result += "Medelvärde för sekunderna Broadmark mätte: \t\t"       + std::to_string(averageThereN)                                                 + " sekunder\n";
		result += "\tStandardavvikelse Broadmark mätte: \t\t"             + std::to_string(therestd)                                                      + " sekunder\n";
		result += "\tVarianesnen Broadmark mätte: \t\t\t"                 + std::to_string(thereVarians)                                                  + " sekunder\n";
		result += "\tmax/min Broadmark uppmätt: \t\t\t"                   + std::to_string(thereN_GLOBAL_MAX) + " / " + std::to_string(thereN_GLOBAL_MIN) + " sekunder\n";


		result += "Medelvärde för procentuella skillnaden mellan dem: \t" + std::to_string(averageDifN)                                                   + " %\n";
		result += "============================================================================================== \n";
		result += "\n";

		std::cout << result << std::endl;

		result += text;



		std::fstream resultFile;
		resultFile.open(path + "result" + fileType, std::ios::app);
		if (resultFile.is_open()) {
			resultFile << result;
			resultFile.close();
		}

	}


	void StartTimer() { start = std::chrono::high_resolution_clock::now(); }
	void EndTimer()   { end   = std::chrono::high_resolution_clock::now(); }
	void PrintToTestRun(double dt)
	{


		std::ofstream validationFile(outputPath,std::ios::app);

		if (validationFile.is_open())
		{
			long vdtl         = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			const double nano = 1000000000.0;
			double vdt        = vdtl / nano;
			double dif        = 1.0 - dt / vdt;

			validationFile << "Min uppmättna tid: " << vdt << "\t Broadmarks uppmättna tid: " << dt << "\t procentuell skillnad mellan tiderna: " << dif << std::endl;
			validationFile.close();
		}
		else
			std::cout << "\n" << "Kunde inte öppna filen: " << outputPath << "\n";
	}


	// skapar en ny testrun fil för denna omgång
	void SetOutputFile()
	{
		bool run      = true;
		int fileCount = 0;
		while (run)
		{
			++fileCount;

			std::string file = path + fileName + std::to_string(fileCount) + fileType;
			run = false;
			if (std::ifstream(file, std::ios::in))
				run = true;

		}

		outputPath = path + fileName + std::to_string(fileCount) + fileType;
	
	}
private:
	std::string outputPath;
	const std::string path     = "D:/SKOL_ARBETE/Kurser/ar_3/Vt/Ex_Jobb/_textFiller/DEL2_genomforande/Validering_utav_broadmark/valideringenUtavBroadMark/";
	const std::string fileName = "testRun";
	const std::string fileType = ".txt";
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
};