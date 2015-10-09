// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Object.h"
#include "CommonEnums.h"
#include "Environment.h"
#include "EnvironmentSpawner.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API AEnvironmentSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	* Spawns the given Environment into the game world.
	* @param Environment reference to the environment that will be spawned into the world
	*/
	TSharedPtr<TArray<APawn*>> SpawnEnvironment(const UEnvironment& Environment);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment")
	TArray<TSubclassOf<AActor>> EnvironmentActorsClasses;
	
};
