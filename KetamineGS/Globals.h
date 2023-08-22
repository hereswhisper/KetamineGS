#pragma once
#include <Windows.h>
#include "SDK.hpp"

using namespace SDK;

namespace Globals {
	UGameEngine* Engine;
	UWorld* World;
	APlayerController* Controller;
	UClass* DefaultPawnClass;
	UGameplayStatics* GameplayStatics;
	AOnlineBeaconHost* Beacon;
	UNetDriver* NetDriver;
	UKismetSystemLibrary* System;
}