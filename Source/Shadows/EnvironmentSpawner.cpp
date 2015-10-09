// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "EnvironmentSpawner.h"

TSharedPtr<TArray<APawn*>> AEnvironmentSpawner::SpawnEnvironment(const UEnvironment& Environment)
{
	TSharedPtr<TArray<APawn*>> StartingPlayerPawns = MakeShareable(new TArray<APawn*>());

	UWorld* World = AActor::GetWorld();
	if (World)
	{
		FVector Location = FVector(0, 0, 0);
		FRotator Rotation = FRotator(0, 0, 0);
		// Spawn a test wall actor to get its dimensions, then use it to determine the scale
		// NOTE: There may be a better way to get a wall's dimensions.
		AActor* TestWallActor = World->SpawnActor(EnvironmentActorsClasses[static_cast<uint8>(EEnvironmentActorType::Wall)],
			&Location,
			&Rotation);
		FVector WallOrigin, WallBoxExtent;
		TestWallActor->GetActorBounds(true, WallOrigin, WallBoxExtent); // Get dimensions of the wall actor
		FVector WallScale = TestWallActor->GetActorScale(); // Needed for the scale of the nav mesh
		World->DestroyActor(TestWallActor); // Remove it from the level

		uint32 Z = 0;
		const TArray<TArray<TArray<UEnvironmentNode*>>>& Nodes = Environment.GetNodes();
		// Iterate through the Environment, spawning the appropriate Actors
		for (auto& Floor : Nodes)
		{
			uint32 Y = 0;
			for (auto& Row : Floor)
			{
				uint32 X = 0;
				for (auto& Node : Row)
				{
					const TArray<FEnvironmentActorInfo>& NodeActors = Node->GetEnvironmentActors();
					for (auto& ActorInfo : NodeActors)
					{
						FVector LocalNodeLocation = ActorInfo.NormLocation * WallBoxExtent * 2;
						FVector Location = FVector(X, Y, Z) * WallBoxExtent * 2 + LocalNodeLocation;
						FRotator Rotation = ActorInfo.Rotation;
						UClass* EnvironmentActorClass = EnvironmentActorsClasses[static_cast<uint8>(ActorInfo.ActorType)];
						if (EnvironmentActorClass)
						{
							AActor* SpawnedActor = World->SpawnActor(EnvironmentActorClass,
								&Location,
								&Rotation);

							if (ActorInfo.ActorType == EEnvironmentActorType::Player)
							{
								APawn* StartPawn = Cast<APawn>(SpawnedActor);
								if (StartPawn)
								{
									StartingPlayerPawns->Add(StartPawn);
								}
							}
						}
					}
					X++;
				}
				Y++;
			}
			Z++;
		}

		// Search thru the actors to find the nav mesh so that it's location and scale can be update
		// to fit with the generated environment.
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			ANavMeshBoundsVolume* NavMeshBoundsVolume = Cast<ANavMeshBoundsVolume>(*ActorItr);
			if (NavMeshBoundsVolume)
			{
				// Set the bounds of the nav mesh to match that of the generated environment.
				// Note: This assumes that the environment has a rectanglular shape.
				int32 NumElementsX = Nodes[0][0].Num();
				int32 NumElementsY = Nodes[0].Num();
				int32 NavMeshScaleX = NumElementsX * WallScale.X / 2;
				int32 NavMeshScaleY = NumElementsY * WallScale.Y / 2;

				NavMeshBoundsVolume->SetActorScale3D(FVector(NavMeshScaleX, NavMeshScaleY, 30));
				NavMeshBoundsVolume->SetActorLocation(FVector(NumElementsX * WallBoxExtent.X, NumElementsY * WallBoxExtent.Y, 0));
				UNavigationSystem* NavSys = World->GetNavigationSystem();
				if (NavSys)
				{
					NavSys->OnNavigationBoundsUpdated(NavMeshBoundsVolume);
				}
			}
		}
	}
	return StartingPlayerPawns;
}


