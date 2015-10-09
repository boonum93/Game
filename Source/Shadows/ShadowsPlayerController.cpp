// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "ShadowsPlayerController.h"

AShadowsPlayerController::AShadowsPlayerController(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AShadowsPlayerController::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shadows Player Controller made."));
}

void AShadowsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MovePawnToLocation", EInputEvent::IE_Pressed, this, &AShadowsPlayerController::MovePawnToLocation);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AShadowsPlayerController::ContextTouch);
}

void AShadowsPlayerController::MovePawnToLocation()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	MoveToLocation(HitResult);
}

void AShadowsPlayerController::ContextTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	MoveToLocation(HitResult);
}

void AShadowsPlayerController::MoveToLocation(FHitResult HitResult)
{
	if (HitResult.bBlockingHit)
	{
		const UNavigationSystem* NavSys = AActor::GetWorld()->GetNavigationSystem();
		if (NavSys)
		{
			NavSys->SimpleMoveToLocation(this, HitResult.ImpactPoint);
		}
	}
}


