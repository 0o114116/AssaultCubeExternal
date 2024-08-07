#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

uint32_t getProcId(const wchar_t* procName);
uintptr_t getModuleBaseAddr(uint32_t procId, const wchar_t* procName);
uintptr_t findDMAAddy(HANDLE procHandle, uintptr_t ptr, const std::vector<unsigned int>& offsets);

class Process
{
	uint32_t mId;
	uintptr_t mBaseAddr{};
	HANDLE mHandle{ nullptr };
public:
	explicit Process(const wchar_t* modlName)
	{
		mId = getProcId(modlName);

		if (mId)
		{
			mBaseAddr = getModuleBaseAddr(mId, modlName);
			mHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, mId);
		}
	}
	
	~Process() { CloseHandle(mHandle); }
	auto getId() const { return mId; }
	auto getBaseAddr() const { return mBaseAddr; }
	auto getHandle() const { return mHandle; }
};