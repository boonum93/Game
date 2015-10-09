// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "Human.h"


// Sets default values
AHuman::AHuman(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 2000.0f;
	// Enables the camera boom to rotate with the controller input
	CameraBoom->bUsePawnControlRotation = true;
	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	Camera->AttachTo(CameraBoom);

	// Prevents the pawn from rotating with the controller input
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	minCameraBoomLength = 400.0f;
	maxCameraBoomLength = 2700.0f;

}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHuman::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent)
		InputComponent->BindAxis("CameraYaw", this, &AHuman::RotateCameraYaw);
	InputComponent->BindAxis("CameraPitch", this, &AHuman::RotateCameraPitch);
	InputComponent->BindAxis("Forward", this, &AHuman::MoveForward);

	InputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &AHuman::CameraZoomIn);
	InputComponent->BindAction("ZoomOut", EInputEvent::IE_Pressed, this, &AHuman::CameraZoomOut);
}

void AHuman::RotateCameraYaw(float InputValue)
{
	if (InputValue != 0.0f)
	{
		AddControllerYawInput(InputValue);
	}
}

void AHuman::RotateCameraPitch(float InputValue)
{
	if (InputValue != 0.0f)
	{
		AddControllerPitchInput(InputValue);
	}
}

void AHuman::MoveForward(float InputValue)
{
	if ((Controller != NULL) && (InputValue != 0.0f))
	{
		FRotator ControllerRotation = Controller->GetControlRotation();
		ControllerRotation.Pitch = 0;
		ControllerRotation.Roll = 0;
		AActor::SetActorRotation(ControllerRotation);


		// find out which way is forward
		FRotator Rotation = AActor::GetActorRotation();//Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}

		//FMath::Lerp(Rotation, ControllerRotation);

		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
	}
}

void AHuman::CameraZoomIn()
{
	if (CameraBoom->TargetArmLength <= minCameraBoomLength)
	{
		// Clamp the minimum length of the camera boom
		CameraBoom->TargetArmLength = minCameraBoomLength;
	}
	else
	{
		// Shorten the camera boom if will not move lower than the minimum length
		CameraBoom->TargetArmLength -= 100;
	}
}

void AHuman::CameraZoomOut()
{
	if (CameraBoom->TargetArmLength >= maxCameraBoomLength)
	{
		// Clamp the maximum length of the camera boom
		CameraBoom->TargetArmLength = maxCameraBoomLength;
	}
	else
	{
		// Lengthen the camera boom if it does not exceed the max length
		CameraBoom->TargetArmLength += 100;
	}
}

