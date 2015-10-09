// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "StartPawn.h"

AStartPawn::AStartPawn(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60, 0, 0));
	

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachTo(CameraBoom);
}