#pragma once

#include <vector>

template <typename T>
class MemAddr
{
public:
	std::vector<unsigned int> mOffsets{};
	uintptr_t mAddress{};
	T mNewValue;
	T mValue{ 0 };

	MemAddr() = default;

	MemAddr(const Process& process, const std::vector<unsigned int> offsets, T newVal)
	{
		mOffsets = offsets;
		mNewValue = newVal;
		mAddress = findDMAAddy(process.getHandle(), process.getBaseAddr() + 0x00110A50, mOffsets);
	}
};