// KetaClient | lol
#include <Windows.h>
#include "SDK.hpp"

using namespace SDK;

UGameEngine* Engine;
UWorld* World;
APlayerController* Controller;
UGameplayStatics* Statics;
UKismetSystemLibrary* KismetSystem;

DWORD WINAPI LolThread(LPVOID) {
	while (true) {
		if (GetAsyncKeyState(VK_F1) & 1) {
			std::string lol = ""; // put pawn class here (AUTOMATIC soon).
			Controller = Engine->GameInstance->LocalPlayers[1]->PlayerController;
			KismetSystem->ExecuteConsoleCommand(World, L"viewclass Pawn", Controller);
		}
	}
}

DWORD WINAPI LolEntry(LPVOID) {
	InitGObjects();

	Engine = UObject::FindObject<UGameEngine>("GameEngine Transient.GameEngine_2147482619");

	World = Engine->GameViewport->World;

	Controller = Engine->GameInstance->LocalPlayers[0]->PlayerController;

	Statics = UObject::FindObject<UGameplayStatics>("GameplayStatics Engine.Default__GameplayStatics");

	KismetSystem = UObject::FindObject<UKismetSystemLibrary>("KismetSystemLibrary Engine.Default__KismetSystemLibrary");

	Controller->CheatManager = ((UCheatManager*)Statics->SpawnObject(UCheatManager::StaticClass(), Controller));

	// Thread goes here
	CreateThread(0, 0, LolThread, 0, 0, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, LolEntry, 0, 0, 0);
		break;
	}
	return TRUE;
}

