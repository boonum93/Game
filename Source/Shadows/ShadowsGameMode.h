// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "EnvironmentFactory.h"
#include "EnvironmentSpawner.h"
#include "ShadowsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API AShadowsGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AShadowsGameMode(const FObjectInitializer& ObjectInitializer);

	/** @return GameSession class to use for this game  */
	virtual TSubclassOf<class AGameSession> GetGameSessionClass() const override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment")
	TSubclassOf<AEnvironmentSpawner> EnvironmentSpawnerClass;
private:
//	APlayerController* PlayerController;
	UEnvironmentFactory* EnvironmentFactory;
	AEnvironmentSpawner* EnvironmentSpawner;
//	UEnvironmentController* EnvController;
	
	
};
