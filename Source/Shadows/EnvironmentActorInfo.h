// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonEnums.h"
#include "EnvironmentActorInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct FEnvironmentActorInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		EEnvironmentActorType ActorType;

	UPROPERTY()
		FVector NormLocation; // Normalized location within block space

	UPROPERTY()
		FRotator Rotation;

	FEnvironmentActorInfo()
	{
		ActorType = EEnvironmentActorType::Empty;
		NormLocation = FVector(0, 0, 0);
		Rotation = FRotator(0, 0, 0);
	}

	FEnvironmentActorInfo(EEnvironmentActorType ActorType, FVector NormLocation, FRotator Rotation)
	{
		this->ActorType = ActorType;
		this->NormLocation = NormLocation;
		this->Rotation = Rotation;
	}
	
};
