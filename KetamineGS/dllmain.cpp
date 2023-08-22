// Ketamine GS | Do not leak faggot
#include <Windows.h>
#include "SDK.hpp"
#include "MinHook.h"
#include "Globals.h"
#include "Helpers.h"
#include <vector>
#include <fstream>
#include "Hooks.h"
#include <format>
using namespace SDK;

void (*ProcessEvent)(UObject*, UFunction*, void*);

// To block
std::vector<std::string>CommonNames = {
	"Function Engine.CameraModifier.BlueprintModifyPostProcess",
	"Function Engine.CameraModifier.BlueprintModifyCamera",
	"Function UMG.UserWidget.OnMouseMove",
	"Function UMG.UserWidget.OnMouseLeave",
	"Function UMG.UserWidget.OnMouseEnter",
};

std::vector<std::string>CommonNameBlocks = {
	"okay1.okay1_C",
	"ALS_HUD.ALS_HUD_C",
	"Base_AI_Enemy.Base_AI_Enemy_C",
	"Engine.AnimInstance",
	"ALS_AnimBP",
	"Main_AI_Controller.Main_AI_Controller_C",
	"PhysXVehicles",
	"okay2.okay2_C",
	"okay.okay_C",
	"ALS_PlayerCameraBehavior.ALS_PlayerCameraBehavior_C",
	"EnemySword_ABP.EnemySword_ABP_C",
	"Ammo.Ammo_C",
	"UMG",
	"ALS_Base_CharacterBP",
	"Health.Health_C",
	"ALS_AnimBP",
	"Base_AI_Enemy.ALS_AnimBP",
	"AIModule",
	"MrKrabsBP.MrKrabsBP_C",
	"Engine",
	"EvaluateGraphExposedInputs_ExecuteUbergraph_ALS_AnimBP",
	"ALS_PlayerCameraManager.ALS_PlayerCameraManager_C",
	"okay2.okay2_C",
	"ALS_PlayerCameraBehavior.OnActorBump",
	"okay2.okay2_C.ReceiveTick",
	"ALS_PlayerCameraBehavior.ALS_PlayerCameraBehavior_C.BlueprintUpdateAnimation",
	"okay.okay_C.ReceiveTick",
	"Base_AI_Enemy.Base_AI_Enemy_C.ReceiveTick",
	"EngineFunction ALS_PlayerCameraBehavior.Function"
};

// NOTES:
// Pawn = MrKrabsBP.MrKrabsBP_C

/*
Function: Function Engine.GameModeBase.SpawnDefaultPawnFor
Function: Function Engine.GameModeBase.SpawnDefaultPawnAtTransform
Function: Function MrKrabsBP.MrKrabsBP_C.ReceiveTick
Function: Function Engine.PlayerController.ServerShortTimeout
*/

static bool isServer = false;
static bool isListening = false;
static bool (*InitHost)(AOnlineBeaconHost*) = decltype(InitHost)(Helpers::GetOffset(0x966260)); // here is inithost



void ProcessEventHook(UObject* Object, UFunction* Function, void* Params) {
	// MainMenu.MainMenu_C.BndEvt__Button_85_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature (Start Button)

	if (!Object || !Function)
		return ProcessEvent(Object, Function, Params);

	auto FuncName = Function->GetFullName();
	auto FuncNameW = FuncName.c_str();

	bool showFunc = true;

	for (int i = 0; i < CommonNames.size(); i++) {
		if (CommonNames[i] == Function->GetFullName()) {
			showFunc = false;
		}
	}

	for (int i = 0; i < CommonNameBlocks.size(); i++) {
		if (strstr(Function->GetFullName().c_str(), CommonNameBlocks[i].c_str())) {
			showFunc = false;
		}
	}

	if (GetAsyncKeyState(VK_F1) & 1) {
		for (int i = 0; i < UObject::GObjects->Num(); i++) {
			auto Obj = UObject::GObjects->GetByIndex(i);

			if (strstr(Obj->GetFullName().c_str(), "MrKrabsBP") || strstr(Obj->GetFullName().c_str(), "MrKrabsBP.MrKrabsBP_C") || strstr(Obj->GetFullName().c_str(), "Server")) {
				std::cout << "Obj Dump: " << Obj->GetFullName() << std::endl;
			}
		}
	}

	if (GetAsyncKeyState(VK_F2) & 1) {
		if (!isListening) {
			isListening = true;
		}
	}

	if (GetAsyncKeyState(VK_F3) & 1) {
		if (!isListening) {
			isListening = true;

			std::cout << "Enabling hooks" << std::endl;

			auto aa = Helpers::GetOffset(0x200DB20);

			MH_CreateHook((void*)aa, Hooks::GetNetModeHook, (void**)&Hooks::GetNetMode);
			MH_EnableHook((void*)aa);
		}
	}

	if (strstr(FuncNameW, "Server") || strstr(FuncNameW, "Krabs") || strstr(FuncNameW, "Connect") || strstr(FuncNameW, "Spawn")) {

		if (strstr(FuncNameW, "PhysXVehicles.WheeledVehicleMovementComponent") || strstr(FuncNameW, "MrKrabsBP.MrKrabsBP_C.ReceiveTick"))
			return ProcessEvent(Object, Function, Params);

		std::cout << "Function: " << Function->GetFullName() << std::endl;
	}

	if (strstr(FuncNameW, "ServerAcknowledgePossession")) {
		auto Paramaters = (Params::APlayerController_ServerAcknowledgePossession_Params*)Params;
		auto Controller = (APlayerController*)Object;
		Controller->AcknowledgedPawn = Paramaters->P;
	}

	if (strstr(FuncNameW, "ServerUpdateLevelVisibility")) {
		Globals::World = Globals::Engine->GameViewport->World;
		std::cout << "Server is listening on 7777." << std::endl;
	}

	if (strstr(FuncNameW, "ServerUpdateCamera")) {
		auto Paramaters = (Params::APlayerController_ServerUpdateCamera_Params*)Params;
		auto PC = (APlayerController*)Object;
	}

	if (strstr(FuncNameW, "SpawnPlayActor")) {
		std::cout << "SpawnPlayer" << std::endl;
	}

	if (strstr(FuncNameW, "ReadyToStartMatch") || strstr(FuncNameW, "ServerUpdateLevelVisibility")) {
		//AOnlineBeaconHost
	}

	if (strstr(FuncNameW, "HandleStartingNewPlayer")) {
		std::cout << "HandleStartingNewPlayer" << std::endl;
		auto BaseTransform = FTransform{};

		FTransform a = BaseTransform;

		a.Translation = { 0, 200, 1000 };
		a.Scale3D = FVector{ 1, 1, 1 };
		a.Rotation = { 0, 0, 0 };
		Globals::World = Globals::Engine->GameViewport->World; // just incase if this goes null
		auto Paramaters = (Params::AGameModeBase_HandleStartingNewPlayer_Params*)Params;
		if (isServer == false) {
			isServer = true;
		}
		else {

			/* auto PlayerControllerDef = Globals::GameplayStatics->BeginDeferredActorSpawnFromClass(Globals::World, AALS_Player_Controller_C::StaticClass(), a, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, Paramaters->NewPlayer);
			 PlayerControllerDef = Globals::GameplayStatics->FinishSpawningActor(PlayerControllerDef, a);

			 Paramaters->NewPlayer = (AALS_Player_Controller_C*)PlayerControllerDef;*/
		}

		std::cout << "Calling here" << std::endl;
		auto PlayerController = Paramaters->NewPlayer;
		auto LOLController = ((AALS_Player_Controller_C*)PlayerController);

		LOLController->bAlwaysRelevant = true;
		LOLController->bReplicates = true;

		LOLController->CheatManager = ((UCheatManager*)Globals::GameplayStatics->SpawnObject(UCheatManager::StaticClass(), LOLController));

		if (LOLController == Globals::World->OwningGameInstance->LocalPlayers[0]->PlayerController) {
			return ProcessEvent(Object, Function, Params);
		}

		if (LOLController->PlayerState) {
			std::cout << "Suck me daddy: " << LOLController->PlayerState->GetFullName() << std::endl;
		}

		if (LOLController) {
			std::cout << "Suck me daddy 2: " << LOLController->GetFullName() << std::endl;

		}

		std::cout << "World: " << __int64(Globals::World) << std::endl;

		auto Pawn = Globals::GameplayStatics->BeginDeferredActorSpawnFromClass(Globals::World, AALS_AnimMan_CharacterBP_C::StaticClass(), a, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, LOLController);
		Pawn = Globals::GameplayStatics->FinishSpawningActor(Pawn, a);

		std::cout << "Pawn Offset: " << __int64(Pawn) << std::endl;

		auto newPlayerPawn = ((AMrKrabsBP_C*)Pawn);
		
		newPlayerPawn->Owner = LOLController;
		newPlayerPawn->OnRep_Owner();

		newPlayerPawn->Controller = LOLController;
		newPlayerPawn->OnRep_Controller();

		newPlayerPawn->PlayerState = LOLController->PlayerState;
		newPlayerPawn->OnRep_PlayerState();

		newPlayerPawn->bAlwaysRelevant = true;
		newPlayerPawn->bReplicates = true;

		//LOLController->PlayerCameraManager = (AALS_PlayerCameraManager_C*)CameraManagerDef;

		Sleep(3000); // maybe this gets called to early :/

		LOLController->Pawn = newPlayerPawn;
		LOLController->AcknowledgedPawn = newPlayerPawn;
		LOLController->Character = newPlayerPawn;
		LOLController->OnRep_Pawn();
		LOLController->Possess(newPlayerPawn);
		newPlayerPawn->bReplicateMovement = true;
		newPlayerPawn->CharacterMovement->bReplicates = true;
		newPlayerPawn->SetReplicateMovement(true);
		PlayerController->OnRep_ReplicateMovement();
		PlayerController->OnRep_ReplicatedMovement();
		newPlayerPawn->OnRep_ReplicateMovement();
		newPlayerPawn->OnRep_ReplicatedMovement();

		if (LOLController->NetConnection)
			std::cout << "bite me mommy: " << LOLController->NetConnection->GetFullName() << std::endl;

		LOLController->NetConnection->ViewTarget = newPlayerPawn;


		std::string t = LOLController->Pawn->GetName();
		std::cout << "Pawn Class: " << t << std::endl;
		std::wstring wstr(t.begin(), t.end());
		std::wstring concatenatedStr = L"viewclass" + wstr;
		auto aaaaa = FString(concatenatedStr.c_str());
		Globals::System->ExecuteConsoleCommand(Globals::World, aaaaa, LOLController);

		Globals::System->ExecuteConsoleCommand(Globals::World, L"camera thirdperson", LOLController);


		if (LOLController->PlayerCameraManager) {
			std::cout << "CAMERA MANAGER!!!!: " << LOLController->PlayerCameraManager->GetFullName() << std::endl;
			std::cout << "Test" << std::endl;
			LOLController->SetViewTargetWithBlend(newPlayerPawn, 1.0, EViewTargetBlendFunction::VTBlend_EaseIn, 1, true);
			((AALS_PlayerCameraManager_C*)LOLController->PlayerCameraManager)->OnPossess(newPlayerPawn);
			((AALS_PlayerCameraManager_C*)LOLController->PlayerCameraManager)->bUseClientSideCameraUpdates = true;
			((AALS_PlayerCameraManager_C*)LOLController->PlayerCameraManager)->TargetCameraLocation = newPlayerPawn->K2_GetActorLocation();

			// this clearly doesn't work.
			((AALS_PlayerCameraManager_C*)LOLController->PlayerCameraManager)->AnimCameraActor->K2_AttachToActor(newPlayerPawn, FName(), EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		}



		//Pawn->Owner = PlayerController;
		//Pawn->OnRep_Owner();

		//((AMrKrabsBP_C*)Pawn)->PlayerState = PlayerController->PlayerState;
		//((AMrKrabsBP_C*)Pawn)->Controller = PlayerController;
		//Pawn->bAlwaysRelevant = true;
		//PlayerController->bAlwaysRelevant = true;
		//PlayerController->Pawn = (AMrKrabsBP_C*)Pawn;
		//LOLController->AcknowledgedPawn = (AMrKrabsBP_C*)Pawn;
		//PlayerController->Possess((AMrKrabsBP_C*)Pawn);
		//PlayerController->Character = (AMrKrabsBP_C*)Pawn;
		//PlayerController->OnRep_Pawn();
		//LOLController->Possess((AMrKrabsBP_C*)Pawn);
		//LOLController->ServerAcknowledgePossession((AMrKrabsBP_C*)Pawn); // Try that aswell
		//PlayerController->OnRep_ReplicateMovement();
		//PlayerController->OnRep_ReplicatedMovement();
		//Pawn->OnRep_ReplicateMovement();
		//Pawn->OnRep_ReplicatedMovement();
	}


	return ProcessEvent(Object, Function, Params);
}



void InitializeOthers() {
	// GameEngine Transient.GameEngine_2147482619
	Globals::Engine = UObject::FindObject<UGameEngine>("GameEngine Transient.GameEngine_2147482619"); // Engine?

	std::cout << "Engine Offset: " << __int64(Globals::Engine) << std::endl; // AHHH

	Globals::DefaultPawnClass = UObject::FindClassFast("/Game/MrKrabs/MrKrabsBP.MrKrabsBP_C");

	std::cout << "DefaultPawnClass Offset: " << __int64(Globals::DefaultPawnClass) << std::endl; // AHHH

	Globals::World = Globals::Engine->GameViewport->World;

	std::cout << "World Offset: " << __int64(Globals::World) << std::endl; // lets hope

	Globals::GameplayStatics = UObject::FindObject<UGameplayStatics>("GameplayStatics Engine.Default__GameplayStatics");

	std::cout << "GameplayStatics Offset: " << __int64(Globals::GameplayStatics) << std::endl; // lets hope 2

	Globals::System = UObject::FindObject<UKismetSystemLibrary>("KismetSystemLibrary Engine.Default__KismetSystemLibrary");
}

// Initalizes UE4 Console
void InitConsole() {
	Globals::Engine->GameViewport->ViewportConsole = reinterpret_cast<UConsole*>(((UGameplayStatics*)UGameplayStatics::StaticClass())->SpawnObject(UConsole::StaticClass(), Globals::Engine->GameViewport));
}

std::ofstream BestestLogger;

DWORD WINAPI KetamineEntry(LPVOID) {
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	BestestLogger.open("Bestest Logur.log");

	SetConsoleTitleA("Ketamine Gameserver | by WhisperEnds");

	MH_Initialize();
	InitGObjects();

	InitializeOthers();

	auto ProcessEventAddr = Helpers::GetOffset(Offsets::ProcessEvent);

	MH_CreateHook((void*)ProcessEventAddr, ProcessEventHook, (void**)&ProcessEvent);
	MH_EnableHook((void*)ProcessEventAddr);

	// 0x200DB20
	//Globals::Engine->GameInstance->LocalPlayers[0]->PlayerController->SwitchLevel(L"ALS_DemoLevel?listen");

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, KetamineEntry, 0, 0, 0);
		break;
	}
	return TRUE;
}

