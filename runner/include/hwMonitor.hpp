#ifndef HWMONITOR_HPP
#define HWMONITOR_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <numeric>
#include <stdlib.h>
#include "Directory.hpp"
#include "File.hpp"
#include "Logging.hpp"

#ifdef _WIN32
#include <windows.h>

#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <wbemidl.h>

//#pragma comment(lib, "wbemuuid.lib")

#elif __linux__
#endif // _WIN32

class hwMonitor
{
	public: 

		hwMonitor();
		std::string GetHwInformation();
		void ParseProgressLine(std::string progressLine);

#ifdef _WIN32

		int GetWMIdata();

#elif __linux__


#endif // _WIN32

	private:

		std::vector<std::string> utilizations;
		std::vector<std::string> temperatures;

		int deviceCount;

		typedef struct
		{
			std::string type;
			std::string name;
		} device;

		typedef struct
		{
			std::string name;
			std::string version;
			std::vector<device> devices;
		} platform;

		std::vector<platform> platforms;

		struct systemStats
		{
			std::string cpuUtilization;
			std::string memoryUtilization;
		} systemStats;

#ifdef _WIN32

		std::string freePhysicalMemory;
		std::string totalVisibleMemorySize;

#elif __linux__

		size_t previousIdleTime;
		size_t previousTotalTime;

#endif // _WIN32


		void getHashcatInfo();
	
		/**
		 * @brief   Parses TEMP values from hashcat progress line
		 * @param   progress_line [] Hashcat's progress line
		 */
		void parseTemp(const std::string& progress_line);

		/**
		 * @brief   Parses UTIL values from hashcat progress line
		 * @param   progress_line [] Hashcat's progress line
		 */
		void parseUtil(const std::string& progress_line);

		bool additionalDataSent;

		/**
		 * @brief   Converts integer value to string
		 * @param   integer to be converted
		 */
		std::string intToString(int integer);

#ifdef _WIN32

		//https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa
		/**
		 * @brief   Converts BSTR value to string
		 * @param   bstr to be converted
		 */
		std::string ConvertBSTRToMBS(BSTR bstr);
		std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);

		

#elif __linux__

		void getCpuUtilizationLinux();
		void getTimesFromProcStat(size_t& idle_time, size_t& total_time);
		std::string extractNumberFromMemInfoLine(std::string line);
		void getMemoryUtilizationLinux();

#endif
};

#endif // HWMONITOR_HPP