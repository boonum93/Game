// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "ShadowsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API AShadowsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShadowsPlayerController(const FObjectInitializer& ObjectInitializer);
	void MovePawnToLocation();
	void ContextTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void MoveToLocation(FHitResult HitResult);
	void BeginPlay() override;
protected:
	virtual void SetupInputComponent() override;
};
