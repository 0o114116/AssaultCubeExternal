#include <iostream>
#include <Windows.h>
#include "Memory.h"
#include "Player.h"
#include "Process.h"

int main()
{
	Process assaultCube{ L"ac_client.exe" };
	Player player{assaultCube};
	bool bHealth{ false }, bAmmo{ false }, bRecoil{ false };

	if (!assaultCube.getId())
		return -1;

	DWORD dwExit{ 0 };

	while (GetExitCodeProcess(assaultCube.getHandle(), &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(0x5A /*Z Key*/) & 1)
		{
			bAmmo = !bAmmo;
		}

		if (bAmmo)
		{
			ReadProcessMemory(assaultCube.getHandle()
				, reinterpret_cast<BYTE*>((player.getAssaultRifle()->clip.mAddress))
				, &(player.getAssaultRifle()->clip.mValue)
				, sizeof(player.getAssaultRifle()->clip.mValue)
				, nullptr);

			if (player.getAssaultRifle()->clip.mValue == 1)
				WriteProcessMemory(assaultCube.getHandle()
					, reinterpret_cast<BYTE*>((player.getAssaultRifle()->clip.mAddress))
					, &(player.getAssaultRifle()->clip.mNewValue)
					, sizeof(player.getAssaultRifle()->clip.mNewValue)
					, nullptr);
		}

		if (GetAsyncKeyState(0x58 /*X Key*/) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				Memory::nopEx(reinterpret_cast<BYTE*>(assaultCube.getBaseAddr() + 0x63786), 
					10, assaultCube.getHandle());
			}
			else
			{
				Memory::patchEx(reinterpret_cast<BYTE*>(assaultCube.getBaseAddr() + 0x63786), 
				                // ReSharper disable once CppCStyleCast
				                (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, assaultCube.getHandle());  // NOLINT(clang-diagnostic-cast-qual)
			}
		}

		if (GetAsyncKeyState(0x43) & 1)
			bHealth = !bHealth;

		if (bHealth)
		{
			Memory::patchEx(reinterpret_cast<BYTE*>(player.getHealth()->mAddress), 
							reinterpret_cast<BYTE*>(&(player.getHealth()->mNewValue)), 
							sizeof(player.getHealth()->mNewValue), assaultCube.getHandle());
		}

		Sleep(10);

		if (GetAsyncKeyState(0x46) & 1)
			return 0;
	}
}