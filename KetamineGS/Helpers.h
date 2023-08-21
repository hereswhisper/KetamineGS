#pragma once
#include <Windows.h>
#include "Globals.h"
#include "SDK.hpp"

using namespace SDK;

namespace Helpers {
	uintptr_t GetOffset(uintptr_t Offset)
	{
		return __int64(GetModuleHandleW(0)) + Offset;
	}

	inline AActor* SpawnActor(UClass* StaticClass, FVector Location = { 0.0f, 0.0f, 0.0f }, AActor* Owner = nullptr, FQuat Rotation = { 0, 0, 0 }, ESpawnActorCollisionHandlingMethod Flags = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		Globals::World = Globals::Engine->GameViewport->World; // Just to make sure

		FTransform SpawnTransform;

		SpawnTransform.Translation = Location;
		SpawnTransform.Scale3D = FVector{ 1, 1, 1 };
		SpawnTransform.Rotation = Rotation;

		AActor* FirstActor = Globals::GameplayStatics->BeginDeferredActorSpawnFromClass(Globals::World, StaticClass, SpawnTransform, Flags, Owner);

		if (FirstActor)
		{
			AActor* FinalActor = Globals::GameplayStatics->FinishSpawningActor(FirstActor, SpawnTransform);

			if (FinalActor)
				return FinalActor;
		}

		return nullptr;
	}

	template <typename ActorType>
	inline ActorType* SpawnActor(FVector Location = { 0.0f, 0.0f, 0.0f }, AActor* Owner = nullptr, FQuat Rotation = { 0, 0, 0 }, ESpawnActorCollisionHandlingMethod Flags = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		return (ActorType*)SpawnActor(ActorType::StaticClass(), Location, Owner, Rotation, Flags);
	}

	inline FTransform GetPlayerStart(APlayerController* Controller) {
		TArray<AActor*> OutActors;
		Globals::GameplayStatics->GetAllActorsOfClass(Globals::World, APlayerStartPIE::StaticClass(), &OutActors);

		auto ActorsNum = OutActors.Num();

		auto SpawnTransform = FTransform();
		SpawnTransform.Scale3D = FVector(1, 1, 1);
		SpawnTransform.Rotation = FQuat();
		SpawnTransform.Translation = FVector{ -124461, -116273, 4000 };

		if (ActorsNum != 0)
		{
			auto ActorToUseNum = rand() % ActorsNum;
			auto ActorToUse = OutActors[ActorToUseNum];

			while (!ActorToUse)
			{
				ActorToUseNum = rand() % ActorsNum;
				ActorToUse = OutActors[ActorToUseNum];
			}

			SpawnTransform.Translation = ActorToUse->K2_GetActorLocation();
		}
		else {
			TArray<AActor*> OutActors;
			Globals::GameplayStatics->GetAllActorsOfClass(Globals::World, APlayerStart::StaticClass(), &OutActors);

			auto ActorsNum = OutActors.Num();

			auto SpawnTransform = FTransform();
			SpawnTransform.Scale3D = FVector(1, 1, 1);
			SpawnTransform.Rotation = FQuat();
			SpawnTransform.Translation = FVector{ -124461, -116273, 4000 };

			if (ActorsNum != 0)
			{
				auto ActorToUseNum = rand() % ActorsNum;
				auto ActorToUse = OutActors[ActorToUseNum];

				while (!ActorToUse)
				{
					ActorToUseNum = rand() % ActorsNum;
					ActorToUse = OutActors[ActorToUseNum];
				}

				SpawnTransform.Translation = ActorToUse->K2_GetActorLocation();
			}
		}

		return SpawnTransform;
	}
}