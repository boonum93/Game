// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "ShadowsMainMenuGameMode.h"
#include "ShadowsMenuPlayerController.h"
#include "ShadowsGameSession.h"
#include "Engine/GameInstance.h"


AShadowsMainMenuGameMode::AShadowsMainMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = AShadowsMenuPlayerController::StaticClass();
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AShadowsMainMenuGameMode::GetGameSessionClass() const
{
	return AShadowsGameSession::StaticClass();
}

bool AShadowsMainMenuGameMode::HostOnlineSession(FName SessionName)
{
	UGameInstance* const GameInstance = AActor::GetGameInstance();
	if (GameInstance)
	{
		AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameSession);
		if (ShadowsGameSession)
		{
			ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
			if (LocalPlayer)
			{
				 return ShadowsGameSession->HostSession(LocalPlayer->GetUniqueNetIdFromCachedControllerId(), SessionName, false, true, 4);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get the local player."));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to get the local player."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast game session to AShadowsGameSession."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to cast game session to AShadowsGameSession."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get the game instance."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to get the game instance."));
	}

	return false;
}

void AShadowsMainMenuGameMode::DestroyOnlineSession()
{
	AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameSession);
	if (ShadowsGameSession)
	{
		ShadowsGameSession->DestroySession();
	}
}


