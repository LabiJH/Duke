#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <string>


std::string getFileName(const std::string& path) {
	// Find the last occurrence of the backslash or forward slash
	size_t lastSlash = path.find_last_of("\\/");

	// If no slash is found, return the entire path (it is a file name)
	if (lastSlash == std::string::npos) {
		return path;
	}

	// Return the substring after the last slash
	return path.substr(lastSlash + 1);
}


std::string ProcIDtoProcName(std::uint32_t pid) {

	std::string result = "UNKNOWN";

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // Open up a Handle with full Access
	if (hProcess != NULL) { // if the process exists, proceed.
		char buffer[MAX_PATH];  // MAX_PATH is a Win Internal Variable for the maximum length of a file path
		GetProcessImageFileNameA(hProcess, buffer, MAX_PATH);
		result = std::string(buffer);
		CloseHandle(hProcess);
	}
	result = getFileName(result);
	return result;
}


int main() {
	DWORD processIDS[500], pCount, returnedBytes;  // Process IDs go up to 4 (- 1 Bit) Bytes, as 0xFFFFFFFF is reserved
	std::string procName = "";

	if (EnumProcesses(processIDS,sizeof(processIDS), &returnedBytes))
	{
		pCount = returnedBytes / sizeof(DWORD);
		std::cout << pCount;

		for (size_t i = 0; i <= pCount; i++)
		{
			procName = ProcIDtoProcName(processIDS[i]);
			if (procName != "UNKNOWN")
			{
				std::cout << procName << " has the PID of " << processIDS[i] << std::endl;
			}
		}
	}
	std::cout << "\nPress any key to exit...";
	std::cin.get();
}

