// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "ShadowsMenuPlayerController.h"
#include "ShadowsMainMenuGameMode.h"

AShadowsMenuPlayerController::AShadowsMenuPlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bShowMouseCursor = true;
}

void AShadowsMenuPlayerController::BeginPlay()
{
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);

	// Slate Widget
	MainMenuWidget = SNew(SMainMenuWidget).OwnerPlayerController(this);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Pass our viewport a weak ptr to our widget
	/////Viewport's weak ptr will not give Viewport ownership of Widget
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MainMenuWidget.ToSharedRef()));
}