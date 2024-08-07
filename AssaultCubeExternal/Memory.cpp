#include <Windows.h>
#include "Memory.h"

void Memory::patchEx(BYTE* dest, const BYTE* source, const unsigned int size, const HANDLE procHandle)
{
	DWORD oldProtect;

	VirtualProtectEx(procHandle, dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(procHandle, dest, source, size, nullptr);
	VirtualProtectEx(procHandle, dest, size, oldProtect, &oldProtect);
}

void Memory::nopEx(BYTE* dest, const unsigned int size, const HANDLE procHandle)
{
	auto* nopArray{ new BYTE[size] };

	memset(nopArray, 0x90, size);
	patchEx(dest, nopArray, size, procHandle);
	delete[] nopArray;
}
