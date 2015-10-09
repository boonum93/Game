// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "ShadowsGameSession.h"
#include "ShadowsMenuPlayerController.h"


AShadowsGameSession::AShadowsGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AShadowsGameSession::OnCreateSessionComplete);
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AShadowsGameSession::OnDestroySessionComplete);

		OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AShadowsGameSession::OnFindSessionsComplete);
//		OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AShadowsGameSession::OnJoinSessionComplete);

		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AShadowsGameSession::OnStartSessionComplete);
	}
}

bool AShadowsGameSession::HostSession(TSharedPtr<const FUniqueNetId> UserId,
	FName SessionName,
	bool bIsLAN,
	bool bIsPresence,
	int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
//		CurrentSessionParams.SessionName = SessionName;
//		CurrentSessionParams.bIsLAN = bIsLAN;
//		CurrentSessionParams.bIsPresence = bIsPresence;
//		CurrentSessionParams.UserId = UserId;
		CurrentSessionName = SessionName;
		MaxPlayers = MaxNumPlayers;

		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			HostSettings = MakeShareable(new FOnlineSessionSettings());
			HostSettings->NumPublicConnections = MaxNumPlayers;
			HostSettings->NumPrivateConnections = 0;
			HostSettings->bShouldAdvertise = true;
			HostSettings->bAllowJoinInProgress = true;
			HostSettings->bIsLANMatch = bIsLAN;
			HostSettings->bIsDedicated = false;
			HostSettings->bUsesStats = true;
			HostSettings->bAllowInvites = true;
			HostSettings->bUsesPresence = true;
			HostSettings->bAllowJoinViaPresence = true;
			HostSettings->bAllowJoinViaPresenceFriendsOnly = false;

//			HostSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			return Sessions->CreateSession(*UserId, SessionName, *HostSettings);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Online session is invalid."));
		}
	}
#if !UE_BUILD_SHIPPING
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get the online subsystem."));
		// Hack workflow in development
		OnCreatePresenceSessionComplete().Broadcast(GameSessionName, true);
		return true;
	}
#endif

	return false;
}

void AShadowsGameSession::StartGame()
{
	// start online game locally and wait for completion
	//IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	//if (OnlineSub)
	//{
	//	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	//	if (Sessions.IsValid())
	//	{
	//		UE_LOG(LogOnlineGame, Log, TEXT("Starting session %s on server"), *GameSessionName.ToString());
	//		OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	//		Sessions->StartSession(GameSessionName);
	//	}
	//}
	UWorld* World = AActor::GetWorld();
	if (World)
	{
		World->ServerTravel(FString("/Game/Maps/GameMap"), false, false);
	}
}

void AShadowsGameSession::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId->IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SessionSearchRef = SessionSearch.ToSharedRef();
			// Clear the delegate so that it can be updated with a new one. This allows FindSessions
			// to be called any time. Without it, a crash would occur if a delegate were already
			// present due to the AddOnFindSessionsCompleteDelegate_Handle function call.
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*UserId, SessionSearchRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void AShadowsGameSession::DestroySession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(CurrentSessionName);
		}
		
	}
}

/**
* Delegate fired when a session create request has completed
*
* @param SessionName the name of the session this callback is for
* @param bWasSuccessful true if the async action completed without error, false if there was an error
*/
void AShadowsGameSession::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("OnCreateSessionComplete %s bSuccess: %d"), *SessionName.ToString(), bWasSuccessful);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	OnCreatePresenceSessionComplete().Broadcast(SessionName, bWasSuccessful);
}

/**
* Delegate fired when a session start request has completed
*
* @param SessionName the name of the session this callback is for
* @param bWasSuccessful true if the async action completed without error, false if there was an error
*/
void AShadowsGameSession::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		// tell non-local players to start online game
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = Cast<APlayerController>(*It);
			if (PC && !PC->IsLocalPlayerController())
			{
				PC->ClientStartOnlineSession();
			}
		}
	}
}

/**
* Delegate fired when a destroying an online session has completed
*
* @param SessionName the name of the session this callback is for
* @param bWasSuccessful true if the async action completed without error, false if there was an error
*/
void AShadowsGameSession::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("OnDestroySessionComplete %s bSuccess: %d"), *SessionName.ToString(), bWasSuccessful);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		HostSettings = NULL;
	}
}

void AShadowsGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogOnlineGame, Verbose, TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful);

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			OnFindSessionsComplete().Broadcast(SessionSearch, bWasSuccessful);
		}
	}
}

