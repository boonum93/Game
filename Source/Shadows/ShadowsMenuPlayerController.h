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
	///** The widget class we will use as our menu when the game starts. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	//TSubclassOf<UUserWidget> MenuWidgetClass;

	///** The widget instance that we are using as our menu. */
	//UPROPERTY()
	//UUserWidget* MenuWidget;

	TSharedPtr<SMainMenuWidget> MainMenuWidget;
};
