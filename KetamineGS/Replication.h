#pragma once
#include "SDK.hpp"
#include "Globals.h"
#include <Windows.h>

using namespace SDK;

namespace Replication {
	// Credits to WaybackGS https://github.com/ctrlkohl/WaybackGS/blob/main/Replication.h
	UChannel* (__fastcall* FnCreateChannel)(UNetConnection*, int, bool, int);
	void(__fastcall* FnSetChannelActor)(UActorChannel*, AActor*);
	bool(__fastcall* FnReplicateActor)(UActorChannel*);
	void(__fastcall* FnCloseChannel)(UChannel*);
	void(__fastcall* FnClientSendAdjustment)(APlayerController*);
	void(__fastcall* FnCallPreReplication)(AActor*, UNetDriver*);

	UActorChannel* FindChannel(UNetConnection* Client, AActor* Actor) {
		if (Client == nullptr) {
			return nullptr;
		}

		//Find an existing channel
		for (int i = 0; i < Client->OpenChannels.Num(); i++) {
			UChannel* Ch = Client->OpenChannels[i];
			if (Ch && Ch->Class == UActorChannel::StaticClass()) {
				UActorChannel* ActorCh = (UActorChannel*)Ch;
				if (ActorCh->Actor == Actor) return ActorCh;
			}
		}

		//No Channel found, create one
		UActorChannel* Ch = reinterpret_cast<UActorChannel*>(FnCreateChannel(Client, 2, true, -1));
		if (Ch) {
			FnSetChannelActor(Ch, Actor);
			Ch->Connection = Client;
		}
		return Ch;
	}


	void ReplicateToClient(AActor* Actor, UNetConnection* Client) {
		if (!Client || !Actor) {
			return;
		}
		if (Actor->IsA(APlayerController::StaticClass()) && Client->PlayerController && Actor != Client->PlayerController) {
			return;
		}
		auto Ch = FindChannel(Client, Actor);
		if (!Ch) {
			return;
		}
		FnCallPreReplication(Actor, Client->Driver);
		FnReplicateActor(Ch);
	}
}