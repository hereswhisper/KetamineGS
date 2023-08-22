#pragma once
#include <Windows.h>
#include "SDK.hpp"
#include "Globals.h"
#include "Helpers.h"

using namespace SDK;

namespace Hooks {
	// TODO 
	void HandleStartingNewPlayerHook(UObject* Object, UFunction* Function, void* Params) {

	}

	// 0x200DB20
	uintptr_t(*GetNetMode)(UWorld* World);
	uintptr_t GetNetModeHook(UWorld* World)
	{
		return 0;
	}
}