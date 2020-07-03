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
#include "RunnerUtils.hpp"

#ifdef _WIN32
#include <windows.h>

#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <wbemidl.h>

#elif __linux__

#endif // _WIN32

class hwMonitor
{
	public: 

		/**
		 * @brief	Constructor
		 */
		hwMonitor();

		/**
		 * @brief	Returns hardware information in XML format
		 */
		std::string GetHwInformation();

		/**
		 * @brief	Takes Hashcat's progress line and calls private methods to fill class member variables
		 * @param	progressLine Hashcat's progress line
		 */
		void ParseProgressLine(std::string progressLine);

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
			int cpuUtilization;
			int memoryUtilization;
		} systemStats;

		bool additionalDataSent;

#ifdef _WIN32

		std::string freePhysicalMemory;
		std::string totalVisibleMemorySize;

		uint64_t previousPercentIdleTime;
		uint64_t previousTimeStamp_Sys100NS;

#elif __linux__

		size_t previousIdleTime;
		size_t previousTotalTime;

#endif // _WIN32

		/**
		 * @brief   Generates Hashcat's info output, parses it and obtains platforms/devices information
		 */
		void getHashcatInfo();
	
		/**
		 * @brief   Parses TEMP values from hashcat progress line
		 * @param   progress_line [in] Hashcat's progress line
		 */
		void parseTemp(const std::string& progress_line);

		/**
		 * @brief   Parses UTIL values from hashcat progress line
		 * @param   progress_line [in] Hashcat's progress line
		 */
		void parseUtil(const std::string& progress_line);

		/**
		 * @brief	Performs WMI queries to get system utilization data
		 * @param	percentIdleTime [out] time spent in idle mode since boot
		 * @param	timeStamp_Sys100NS [out] total time spent since boot
		 */
		int GetWMIdata(uint64_t& percentIdleTime, uint64_t& timeStamp_Sys100NS);

		/**
		 * @brief	Converts integer to string
		 * @param	integer [in]
		 */
		std::string intToString(int integer);

#ifdef _WIN32

		/**
		 * @brief	Fills class member variables
		 * 
		 */
		void getCpuAndMemoryUtilizationWindows();

		//https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa
		/**
		 * @brief   Converts BSTR value to string
		 * @param   bstr to be converted
		 */
		std::string ConvertBSTRToMBS(BSTR bstr);
		std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);

		

#elif __linux__

		/**
		 * @brief	Fills systemStats.cpuUtilization, previousIdleTime and previousTotalTime class member variable
		 */
		void getCpuUtilizationLinux();

		/**
		 * @brief	Parses "/proc/stat" file and calculates idle and total time spent since boot
		 * @param	idle_time [out] time spent in idle mode since boot
		 * @param	total_time [out] total time spent since boot
		 */
		void getTimesFromProcStat(size_t& idle_time, size_t& total_time);

		/**
		 * @brief	Extracts number from "/proc/meminfo" file's single line
		 * @param	line [in] "/proc/stat" file's single line
		 */
		std::string extractNumberFromMemInfoLine(std::string line);

		/**
		 * @brief	Fills systemStats.memoryUtilization, previousIdleTime and previousTotalTime class member variable
		 */
		void getMemoryUtilizationLinux();

#endif
};

#endif // HWMONITOR_HPP