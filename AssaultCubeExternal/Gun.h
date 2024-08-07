#pragma once

#include "MemAddr.h"

class Gun
{
public:
	MemAddr<int> clip;
	MemAddr<int> ammo;
	MemAddr<int> shot;
	MemAddr<int> cool;

	Gun() = default;

	Gun(const Process& process, 
		const std::vector<unsigned int>& offClip, 
		const std::vector<unsigned int>& offAmmo,
		const std::vector<unsigned int>& offShot,
		const std::vector<unsigned int>& offCool,
		const int newValClip,
		const int newValAmmo,
		const int newValShot,
		const int newValCool):
		clip(process, offClip, newValClip),
		ammo(process, offAmmo, newValAmmo),
		shot(process, offShot, newValShot),
		cool(process, offCool, newValCool)
	{

	}
};