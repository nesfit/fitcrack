#include "hwMonitor.hpp"

hwMonitor::hwMonitor(void)
{
#ifdef _WIN32
	
#elif __linux__
	previousIdleTime = 0;
	previousTotalTime = 0;
	getTimesFromProcStat(previousIdleTime, previousTotalTime);
#endif // __linux__
	systemStats.cpuUtilization = -1;
	systemStats.memoryUtilization = -1;

	additionalDataSent = false;
}

std::string hwMonitor::GetHwInformation()
{
	std::string XML;
	XML.clear();
	
#ifdef _WIN32
	GetWMIdata();
#elif __linux__
	getCpuUtilizationLinux();
	getMemoryUtilizationLinux();
#endif // _WIN32

	XML += "<hwStats>\n";

		if (!additionalDataSent)
		{
		getHashcatInfo();

		XML += "<platforms>\n";
			int platformCount = platforms.size();
			for (int i = 0; i < platformCount; i++)
			{
				XML += "<platform>\n";
					XML += "<platformName>" + platforms[i].name + "</platformName>\n";
					XML += "<platformVersion>" + platforms[i].version + "</platformVersion>\n";

					XML += "<platformDevices>\n";
						int platformDeviceCount = platforms[i].devices.size();
						for (int n = 0; n < platformDeviceCount; n++)
						{
							XML += "<platformDevice>\n";
								XML += "<deviceType>" + platforms[i].devices[n].type + "</deviceType>\n";
								XML += "<deviceName>" + platforms[i].devices[n].name + "</deviceName>\n";
							XML += "</platformDevice>\n";
						}
					XML += "</platformDevices>\n";
				XML += "</platform>\n";
			}
		XML += "</platforms>\n";

		additionalDataSent = true;
		}

		XML += "<systemStats>\n";
			XML += "<cpuUtil>" + systemStats.cpuUtilization + "</cpuUtil>\n";
			XML += "<memUtil>" + systemStats.memoryUtilization + "</memUtil>\n";
		XML += "</systemStats>\n";

		XML += "<devicesStats>\n";
		for (int i = 0; i < deviceCount; i++)
		{
			XML += "<deviceStats>\n";
				XML += "<util>" + utilizations[i] + "</util>\n";
				XML += "<temp>" + temperatures[i] + "</temp>\n";
				XML += "<index>" + intToString(i+1) + "</index>\n";
			XML += "</deviceStats>\n";
		}
		XML += "</devicesStats>\n";

	XML += "</hwStats>\n";

	return XML;
}

#ifdef _WIN32
	int hwMonitor::GetWMIdata()
	{
		// https://docs.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer
		HRESULT hres;

		// Step 1: --------------------------------------------------
		// Initialize COM. ------------------------------------------

		hres = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(hres))
		{
			return 1;                  // Program has failed.
		}

		// Step 2: --------------------------------------------------
		// Set general COM security levels --------------------------

		hres = CoInitializeSecurity(
			NULL,
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities 
			NULL                         // Reserved
		);


		if (FAILED(hres))
		{
			CoUninitialize();
			return 1;                    // Program has failed.
		}

		// Step 3: ---------------------------------------------------
		// Obtain the initial locator to WMI -------------------------

		IWbemLocator* pLoc = NULL;

		hres = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pLoc);

		if (FAILED(hres))
		{
			CoUninitialize();
			return 1;                 // Program has failed.
		}

		// Step 4: -----------------------------------------------------
		// Connect to WMI through the IWbemLocator::ConnectServer method

		IWbemServices* pSvc = NULL;

		// Connect to the root\cimv2 namespace with
		// the current user and obtain pointer pSvc
		// to make IWbemServices calls.
		hres = pLoc->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			0,                    // Security flags.
			0,                       // Authority (for example, Kerberos)
			0,                       // Context object 
			&pSvc                    // pointer to IWbemServices proxy
		);

		if (FAILED(hres))
		{
			pLoc->Release();
			CoUninitialize();
			return 1;                // Program has failed.
		}

		// Step 5: --------------------------------------------------
		// Set security levels on the proxy -------------------------

		hres = CoSetProxyBlanket(
			pSvc,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name 
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_NONE                    // proxy capabilities 
		);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

		// Step 6: --------------------------------------------------
		// Use the IWbemServices pointer to make requests of WMI ----

		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(
			_bstr_t(L"WQL"),
			_bstr_t(L"SELECT * FROM Win32_OperatingSystem"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

		// Step 7: -------------------------------------------------
		// Get the data from the query in step 6 -------------------

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;

		while (pEnumerator)
		{
			pEnumerator->Next(WBEM_INFINITE, 1,
				&pclsObj, &uReturn);

			if (0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			// Get the value of the TotalVisibleMemorySize property
			pclsObj->Get(L"TotalVisibleMemorySize", 0, &vtProp, 0, 0);
			totalVisibleMemorySize = ConvertBSTRToMBS(vtProp.bstrVal);
			VariantClear(&vtProp);
			// Get the value of the FreePhysicalMemory property
			pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
			freePhysicalMemory = ConvertBSTRToMBS(vtProp.bstrVal);
			VariantClear(&vtProp);
			int ramUtil = (100 - ((atoi(freePhysicalMemory.c_str()) / (double)(atoi(totalVisibleMemorySize.c_str()))) * 100));
			systemStats.memoryUtilization = intToString(ramUtil);

			pclsObj->Release();
		}
		//----------------------
		pEnumerator->Release();
		pEnumerator = NULL;

		hres = pSvc->ExecQuery(
			_bstr_t(L"WQL"),
			_bstr_t(L"SELECT * FROM Win32_Processor"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			return 1;               // Program has failed.
		}

		pclsObj = NULL;
		uReturn = 0;

		while (pEnumerator)
		{
			pEnumerator->Next(WBEM_INFINITE, 1,
				&pclsObj, &uReturn);

			if (0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			// Get the value of the LoadPercentage property
			pclsObj->Get(L"LoadPercentage", 0, &vtProp, 0, 0);
			systemStats.cpuUtilization = intToString(vtProp.intVal);
			VariantClear(&vtProp);


			pclsObj->Release();
		}

		// Cleanup
		// ========

		pSvc->Release();
		pLoc->Release();
		pEnumerator->Release();
		CoUninitialize();

		return 0;   // Program successfully completed.
	}

	std::string hwMonitor::ConvertBSTRToMBS(BSTR bstr)
	{
		int wslen = ::SysStringLen(bstr);
		return ConvertWCSToMBS((wchar_t*)bstr, wslen);
	}

	std::string hwMonitor::ConvertWCSToMBS(const wchar_t* pstr, long wslen)
	{
		int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

		std::string dblstr(len, '\0');
		len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
			pstr, wslen /* not necessary NULL-terminated */,
			&dblstr[0], len,
			NULL, NULL /* no default char */);

		return dblstr;
	}

#elif __linux__

	void hwMonitor::getTimesFromProcStat(size_t& idle_time, size_t& total_time)
	{
		// times in /proc/stat: USER NICE SYSTEM IDLE IOWAIT IRQ SOFTIRQ STEAL GUEST GUEST_NICE
		std::ifstream proc_stat("/proc/stat");
		proc_stat.ignore(5, ' '); // Skip the 'cpu' prefix.
		std::vector<size_t> times;
		size_t time;
		for (int i = 0; i < 8; i++) // Get only first 8 'times', the last ones are already included in previous
		{
			proc_stat >> time;
			times.push_back(time);
		}
		idle_time = times[3] + times[4];
		total_time = std::accumulate(times.begin(), times.end(), 0);
		proc_stat.close();
	}

	void hwMonitor::getCpuUtilizationLinux()
	{
		size_t idle_time = 0, total_time = 0;

		getTimesFromProcStat(idle_time, total_time);

		const float idle_time_delta = idle_time - previousIdleTime;
		const float total_time_delta = total_time - previousTotalTime;
		int cpuUtil = 100.0 * (1.0 - idle_time_delta / total_time_delta);
		systemStats.cpuUtilization = intToString(cpuUtil);
		previousIdleTime = idle_time;
		previousTotalTime = total_time;

	}

	std::string hwMonitor::extractNumberFromMemInfoLine(std::string line)
	{
		size_t pos;
		pos = line.find_last_of(" ");
		line = line.substr(0, pos);
		pos = line.find_last_of(" ");
		line = line.substr(pos + 1);
		return line;
	}

	void hwMonitor::getMemoryUtilizationLinux()
	{
		std::ifstream proc_meminfo("/proc/meminfo");
		std::string MemTotal, MemFree;

		std::getline(proc_meminfo, MemTotal);
		MemTotal = extractNumberFromMemInfoLine(MemTotal);
		std::getline(proc_meminfo, MemFree);
		MemFree = extractNumberFromMemInfoLine(MemFree);
		proc_meminfo.close();
		int memUtil = (100 - ((atoi(MemFree.c_str()) / (double)(atoi(MemTotal.c_str()))) * 100));
		systemStats.memoryUtilization = intToString(memUtil);
	}

#endif

	void hwMonitor::ParseProgressLine(std::string progressLine)
	{
		parseTemp(progressLine);
		parseUtil(progressLine);
	}

	std::string hwMonitor::intToString(int integer)
	{
		std::stringstream intStringStream;
		intStringStream << integer;
		return intStringStream.str();
	}

	void hwMonitor::parseTemp(const std::string& progress_line) {
		size_t last = 0;
		size_t found_at;
		std::string sectionWithTemperatures;
		deviceCount = 0;
		
		temperatures.clear();

		// TEMP\t<N x %d\t>REJECTED
		found_at = progress_line.find("TEMP", last);  // Find position of TEMP

		if (found_at == std::string::npos) {
			return;
		}

		last = found_at;

		found_at = progress_line.find("\t", last);	    // Get position of \t after TEMP
		last = found_at + 1;  // Set position to first character after \t

		found_at = progress_line.find("REJECTED", last);  // Get position of REJECTED

		if (found_at == std::string::npos) {
			return;
		}

		sectionWithTemperatures = progress_line.substr(last, found_at - last);  // Cut the section with temperatures

		last = 0;
		while ((found_at = sectionWithTemperatures.find("\t", last)) != std::string::npos)
		{
			
			temperatures.push_back(sectionWithTemperatures.substr(last, found_at - last)); // Cut out temperature value
			deviceCount++;
			last = found_at + 1;
		}
	}

	void hwMonitor::parseUtil(const std::string& progress_line) {
		size_t last = 0;
		size_t found_at;
		std::string sectionWithUtilizations;

		utilizations.clear();

		found_at = progress_line.find("UTIL", last);  // Find position of UTIL

		if (found_at == std::string::npos) {
			return;
		}

		last = found_at;

		found_at = progress_line.find("\t", last);	    // Get position of \t after TEMP
		last = found_at + 1;  // Set position to first character after \t

		sectionWithUtilizations = progress_line.substr(last);  // Cut the section with temperatures

		last = 0;
		while ((found_at = sectionWithUtilizations.find("\t", last)) != std::string::npos)
		{
			utilizations.push_back(sectionWithUtilizations.substr(last, found_at - last)); // Cut out utilization value
			last = found_at + 1;
		}
	}

	void hwMonitor::getHashcatInfo(){
		File executable;

		Directory directory(".");
		directory.scanForEntities();

		std::string command = "";
		std::string line;

		#ifdef __linux__
			directory.findVersionedFile("hashcat", "bin", executable);
			command += "./";
		#elif _WIN32
			directory.findVersionedFile("hashcat", "exe", executable);
		#endif

		command += executable.getName() + " -I > hashcatInfoOutput.txt";

		int retVal = system(command.c_str());
		if (retVal != 0) return;

		std::ifstream hashcatInfo("hashcatInfoOutput.txt");

		while(getline(hashcatInfo, line))
		{
			if (line.find("Platform") != std::string::npos)
			{
				platform platform;
				// If Platform found, search for its name and version
				while(getline(hashcatInfo, line))
				{
					int position;
					if (line.find("Name") != std::string::npos)
					{
						position = line.find(":");
						platform.name = line.substr(position+2);
						continue;
					}
					if (line.find("Version") != std::string::npos)
					{
						position = line.find(":");
						platform.version = line.substr(position+2);
						break;
					}
				}
				// Ended searching for platform's name and version
				platforms.push_back(platform);
			}
			if (line.find("Device") != std::string::npos)
			{
				// If Device found, search for its type and name
				device device;

				while(getline(hashcatInfo, line))
				{
					int position;
					if (line.find("Type") != std::string::npos)
					{
						position = line.find(":");
						device.type = line.substr(position+2);
						continue;
					}
					if (line.find("Name") != std::string::npos)
					{
						position = line.find(":");
						device.name = line.substr(position+2);
						break;
					}
				}
				// Ended searching for device's name and version
				// Add that device to last found platform
				platforms[platforms.size()-1].devices.push_back(device);
			}
		}
		remove("hashcatInfoOutput.txt");
	}

