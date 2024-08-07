#pragma once

#include <Windows.h>

namespace Memory
{
	void patchEx(BYTE* dest, const BYTE* source, unsigned int size, HANDLE procHandle);
	void nopEx(BYTE* dest, unsigned int size, HANDLE procHandle);
}