// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Human.generated.h"

UCLASS()
class SHADOWS_API AHuman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuman(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void RotateCameraYaw(float InputValue);
	void RotateCameraPitch(float InputValue);
	void MoveForward(float InputValue);
	void CameraZoomIn();
	void CameraZoomOut();

protected:
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

private:
	float minCameraBoomLength;
	float maxCameraBoomLength;
	
};
