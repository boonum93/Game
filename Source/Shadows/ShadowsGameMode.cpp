// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "ShadowsGameMode.h"
#include "ShadowsPlayerController.h"
#include "MazeFactory.h"
#include "ShadowsGameSession.h"

AShadowsGameMode::AShadowsGameMode(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	PlayerControllerClass = AShadowsPlayerController::StaticClass();
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AShadowsGameMode::GetGameSessionClass() const
{
	return AShadowsGameSession::StaticClass();
}

void AShadowsGameMode::BeginPlay()
{
	AGameMode::BeginPlay();

	UWorld* World = AActor::GetWorld();
	if (World)
	{
		// TODO: Handle deletion of these. Actually shouldnt have to. Garbage collection will do it.
		EnvironmentFactory = NewObject<UMazeFactory>();

		FVector Location(0, 0, 0);
		FRotator Rotation(0, 0, 0);
		EnvironmentSpawner = Cast<AEnvironmentSpawner>(World->SpawnActor(EnvironmentSpawnerClass, &Location, &Rotation));
		UEnvironment* Environment = EnvironmentFactory->MakeEnvironment(NumPlayers, 10, 10, 1);
		TSharedPtr<TArray<APawn*>> StartingPlayerPawns = EnvironmentSpawner->SpawnEnvironment(*Environment);

		// Iterate through all player controllers, destroy their default pawns and possess the
		// temporary pawns spawned in from the EnvironmentSpawner.
		int32 PlayerIndex = 0;
		for (FConstPlayerControllerIterator PCItr = World->GetPlayerControllerIterator(); PCItr; ++PCItr)
		{
			AShadowsPlayerController* ShadowsPlayerController = Cast<AShadowsPlayerController>(*PCItr);
			// Unpossess default pawn and destroy it from the world
			APawn* DefaultPawn = ShadowsPlayerController->GetPawn();
			World->DestroyActor(DefaultPawn);

			if (StartingPlayerPawns->IsValidIndex(PlayerIndex))
			{
				ShadowsPlayerController->Possess((*StartingPlayerPawns)[PlayerIndex]);
				// Ignore look and move input so player can select their pawn
				ShadowsPlayerController->SetIgnoreLookInput(true);
				ShadowsPlayerController->SetIgnoreMoveInput(true);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Controller to start pawn mismatch!"));
				}
			}
			
			PlayerIndex++;
		}
	}
}


