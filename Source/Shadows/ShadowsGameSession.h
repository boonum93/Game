// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"
#include "GameFramework/GameSession.h"
#include "ShadowsGameSession.generated.h"

struct FShadowsGameSessionParams
{
	/** Name of session settings are stored with */
	FName SessionName;
	/** LAN Match */
	bool bIsLAN;
	/** Presence enabled session */
	bool bIsPresence;
	/** Id of player initiating lobby */
	TSharedPtr<FUniqueNetId> UserId;
	/** Current search result choice to join */
	int32 BestSessionIdx;

	FShadowsGameSessionParams()
		: SessionName(NAME_None)
		, bIsLAN(false)
		, bIsPresence(false)
		, BestSessionIdx(0)
	{
	}
};

/**
 * 
 */
UCLASS()
class SHADOWS_API AShadowsGameSession : public AGameSession
{
	GENERATED_BODY()

protected:
	/** Delegate for creating a new session */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/** Delegate after starting a session */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/** Delegate after joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FName CurrentSessionName;

	/** Current host settings */
	TSharedPtr<class FOnlineSessionSettings> HostSettings;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	* Delegate fired when a session create request has completed
	*
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* Delegate fired when a session start request has completed
	*
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* Delegate fired when a destroying an online session has completed
	*
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void OnFindSessionsComplete(bool bWasSuccessful);

	/*
	* Event triggered when a presence session is created
	*
	* @param SessionName name of session that was created
	* @param bWasSuccessful was the create successful
	*/
	DECLARE_EVENT_TwoParams(AShadowsGameSession, FOnCreatePresenceSessionComplete, FName /*SessionName*/, bool /*bWasSuccessful*/);
	FOnCreatePresenceSessionComplete CreatePresenceSessionCompleteEvent;

	/*
	* Event triggered when a session is joined
	*
	* @param SessionName name of session that was joined
	* @param bWasSuccessful was the create successful
	*/
	DECLARE_EVENT_OneParam(AShadowsGameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
	FOnJoinSessionComplete JoinSessionCompleteEvent;

	/*
	* Event triggered after session search completes
	* @param SessionSearch the results from the find
	* @param bWasSuccessful was the find successful
	*/
	DECLARE_EVENT_TwoParams(AShadowsGameSession, FOnFindSessionsComplete, TSharedPtr<FOnlineSessionSearch> /*SessionSearch*/, bool /*bWasSuccessful*/);
	FOnFindSessionsComplete FindSessionsCompleteEvent;

public:
	AShadowsGameSession(const FObjectInitializer& ObjectInitializer);

	/**
	* Host a new online session
	*
	* @param UserId user that initiated the request
	* @param SessionName name of session
	* @param bIsLAN is this going to hosted over LAN
	* @param bIsPresence is the session to create a presence session
	* @param MaxNumPlayers Maximum number of players to allow in the session
	*
	* @return bool true if successful, false otherwise
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId,
		FName SessionName,
		bool bIsLAN,
		bool bIsPresence,
		int32 MaxNumPlayers);

	void StartGame();

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);

	void DestroySession();

	/** @return the delegate fired when creating a presence session */
	FOnCreatePresenceSessionComplete& OnCreatePresenceSessionComplete() { return CreatePresenceSessionCompleteEvent; }

	/** @return the delegate fired when search of session completes */
	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionsCompleteEvent; }

private:
	/** Handles to various registered delegates */
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	
	
};
