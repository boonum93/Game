// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SMainMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "ShadowsMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API AShadowsMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShadowsMenuPlayerController(const FObjectInitializer& ObjectInitializer);
	void BeginPlay() override;

protected:
	TSharedPtr<SMainMenuWidget> MainMenuWidget;
};
