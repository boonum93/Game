// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "StartPawn.generated.h"

UCLASS()
class SHADOWS_API AStartPawn : public APawn
{
	GENERATED_BODY()

public:
	AStartPawn(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

};
