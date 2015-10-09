// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "ShadowsMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API AShadowsMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AShadowsMainMenuGameMode(const FObjectInitializer& ObjectInitializer);

	/** @return GameSession class to use for this game  */
	virtual TSubclassOf<class AGameSession> GetGameSessionClass() const override;

	bool HostOnlineSession(FName SessionName);

	void DestroyOnlineSession();

};
