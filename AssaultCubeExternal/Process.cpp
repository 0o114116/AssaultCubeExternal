#include "Process.h"

uint32_t getProcId(const wchar_t* procName)
{
	uint32_t procId{ 0 };
	const HANDLE snapHandle{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	if (snapHandle != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(snapHandle, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapHandle, &procEntry));
		}
	}
	
	CloseHandle(snapHandle);

	return procId;
}

uintptr_t getModuleBaseAddr(uint32_t procId, const wchar_t* procName)
{
	uintptr_t modBaseAddr{ 0 };
	const HANDLE snapHandle{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId) };

	if (snapHandle != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(snapHandle, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, procName))
				{
					modBaseAddr = reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
					break;
				}
			} while (Module32Next(snapHandle, &modEntry));
		}
	}

	CloseHandle(snapHandle);

	return modBaseAddr;
}

uintptr_t findDMAAddy(const HANDLE procHandle, const uintptr_t ptr, const std::vector<unsigned int>& offsets)
{
	uintptr_t addr{ ptr };

	for (const auto off : offsets)
	{
		ReadProcessMemory(procHandle, reinterpret_cast<BYTE*>(addr), &addr, sizeof(addr), nullptr);

		addr += off;
	}

	return addr;
}