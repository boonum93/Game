// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "MazeFactory.h"
#include "EnvironmentNode.h"

UEnvironment* UMazeFactory::MakeEnvironment(int32 NumStartingPoints, int32 Length, int32 Width, int32 Floors)
{
	//TArray<FArray2D<EnvironmentNode>>* Maze = new TArray<FArray2D<EnvironmentNode>>();
	TArray<TArray<TArray<UEnvironmentNode*>>>* Maze = new TArray<TArray<TArray<UEnvironmentNode*>>>();
	// Represents all of the open cells within the maze as disjoint sets. As walls 
	// are removed, cells will be connected and placed in the same set. When all 
	// of the cells are in one set and have one parent, then the maze is created.
	TArray<int32> Parent;
	// Used to prevent a long path to the cell's parent.
	TArray<int32> Rank;
	// Holds all of the removable walls in the maze. Each wall is its own element.
	TArray<int32> Walls;

	for (int32 i = 0; i < Floors; i++)
	{
		//FArray2D<EnvironmentNode>* Floor = new FArray2D<EnvironmentNode>();
		TArray<TArray<UEnvironmentNode*>>* Floor = new TArray<TArray<UEnvironmentNode*>>();
		// Add the correct number of elements to the array so that it is the correct size.
		//Floor->AddUninitialized(2 * Length + 1, 2 * Width + 1);
		// Make the Parent and Rank arrays Height*Width length
		Parent.AddUninitialized(Length * Width);
		Rank.AddUninitialized(Parent.Num());
		// After doing math, the array is given the length of the number of removable walls.
		Walls.AddUninitialized(2 * Length*Width - Width - Length);

		// Initialize the maze array with the starting maze with blocked off cells. 
		// Each wall is represented as a "#" and each cell is represented as a ' '.
		for (int32 i = 0; i < 2 * Length + 1/*Floor->Rows.Num()*/; i++)
		{
			TArray<UEnvironmentNode*>* Row = new TArray<UEnvironmentNode*>();
			for (int32 j = 0; j < 2 * Width + 1/*Floor->Rows[i].Columns.Num()*/; j++)
			{
				UEnvironmentNode* Node = NewObject<UEnvironmentNode>();
				if (i % 2 != 0 && j % 2 != 0)
				{
					Node->AddActor(FEnvironmentActorInfo(EEnvironmentActorType::Floor, FVector(0, 0, -0.5), FRotator(0, 0, 0)));
				}
				else
				{
					Node->AddActor(FEnvironmentActorInfo(EEnvironmentActorType::Wall, FVector(0, 0, 0), FRotator(0, 0, 0)));
				}
				
				Row->Add(Node);
			}
			Floor->Add(*Row);
		}

		// Initializes the Parent and Rank arrays with their re
		MakeSet(Parent, Rank, Parent.Num());

		// Initialize the array by giving each wall a number from 0 to Walls.Num() - 1.
		for (int32 i = 0; i < Walls.Num(); i++)
		{
			Walls[i] = i;
		}
		// Mix up the walls so that the head of the array can be popped off.
		ShuffleArray(Walls);

		// As the walls are removed from the maze array, this will increment.
		int32 NumWallsRemoved = 0;

		// Once NumWallsRemoved reaches the number of original disjoint sets minus 1,
		// we know that all the cells have been connected, then exit the loop.
		while (NumWallsRemoved < Parent.Num() - 1)
		{
			// Remove a wall and allow the array to shrink if need be.
			int32 RemovedWall = Walls.Pop(true);

			int32 P1, P2, M1, M2;

			// There are certain walls that separate cells horizontally and
			// certain walls that separate cells vertically. The cutoff point
			// that differentiates horizontal walls from vertical walls is
			// the expression width * height - height. If the wall's number is
			// less than this cutoff point, then the wall is a horizontal wall 
			// and requires a different set of relation equations from the vertical walls.
			if (RemovedWall < (Width * Length - Length))
			{
				// Horizontal wall relation to the parent array
				P1 = RemovedWall / (Width - 1) + RemovedWall;
				P2 = RemovedWall / (Width - 1) + RemovedWall + 1;
				// Horizontal wall relation to the maze array
				M1 = 2 * (RemovedWall / (Width - 1)) + 1;
				M2 = 2 * (RemovedWall % (Width - 1)) + 2;
			}
			else
			{
				// Vertical wall relation to the parent array.
				P1 = RemovedWall - Width*Length + Length;
				P2 = RemovedWall - Width*Length + Width + Length;

				// Vertical wall relation to the 2D maze array.
				M1 = 2 * (P1 / Width) + 2;
				M2 = 2 * (P1 % Width) + 1;
			}

			if (FindSet(Parent, P1) != FindSet(Parent, P2))
			{
				// Removes the wall from the 2D maze.
				(*Floor)[M1][M2]->RemoveActors(EEnvironmentActorType::Wall);
				(*Floor)[M1][M2]->AddActor(FEnvironmentActorInfo(EEnvironmentActorType::Floor, FVector(0, 0, -0.5), FRotator(0, 0, 0)));
				// Union the two disjoint cells in the parent array.
				Union(Parent, Rank, P1, P2);
				// Increment NumWallsRemoved, because a wall has been removed.
				NumWallsRemoved++;
			}
		}
		Maze->Add(*Floor);
	}


	// Mark the starting point and ending point on the maze.
	for (int32 i = 0; i < NumStartingPoints; i++)
	{
		(*Maze)[0][1][1]->AddActor(FEnvironmentActorInfo(EEnvironmentActorType::Player, FVector(0, 0, 0), FRotator(0, 0, 0)));
	}
	
	(*Maze)[Floors - 1][(*Maze)[Floors - 1].Num() - 2][(*Maze)[Floors - 1][0].Num() - 2]
		->AddActor(FEnvironmentActorInfo(EEnvironmentActorType::ExitDoor, FVector(0, 0, 0), FRotator(0, 0, 0)));

	UEnvironment* Environment = NewObject<UEnvironment>();
	Environment->SetNodes(Maze);
	return Environment;
}

void UMazeFactory::MakeSet(TArray<int32>& Parent, TArray<int32>& Rank, int32 ArrayLength)
{
	for (int32 i = 0; i < ArrayLength; i++)
	{
		Parent[i] = i;
		Rank[i] = 0;
	}
}

int32 UMazeFactory::FindSet(TArray<int32>& Parent, int32 X)
{
	// If the parent of element X is element X, then return X.
	if (Parent[X] == X)
	{
		return X;
	}
	// Recursively calls findset to move closer to the head of the set, 
	// while at the same time using path compression to make the head 
	// of the set the parent of every element in the set that is visited 
	// along the way towards finding the head of the set.
	Parent[X] = FindSet(Parent, Parent[X]);

	return Parent[X];
}

void UMazeFactory::Union(TArray<int32>& Parent, TArray<int32> Rank, int32 X, int32 Y)
{
	// Finds the head of the set for both disjoint sets
	int32 SetX = FindSet(Parent, X);
	int32 SetY = FindSet(Parent, Y);

	// Makes the lower ranked set's head attach underneath the higher 
	// ranked set's head as a way to slow the growth of the path to 
	// finding a set's head. If the sets have the same rank, then one 
	// set is placed under the other and the rank is increased by 1.
	if (Rank[SetX] < Rank[SetY])
	{
		Parent[SetX] = SetY;
	}
	else if (Rank[SetY] < Rank[SetX])
	{
		Parent[SetY] = SetX;
	}
	else
	{
		Parent[SetY] = SetX;
		Rank[SetX]++;
	}
}

void UMazeFactory::ShuffleArray(TArray<int32>& Array)
{
	int32 NumSwaps = Array.Num() / 2;
	int32 RandIndex1, RandIndex2;

	for (int32 i = 0; i < NumSwaps; i++)
	{
		// Gets two random indices whose data will be swapped at the indices.
		RandIndex1 = FMath::RandRange(0, Array.Num() - 1);
		RandIndex2 = FMath::RandRange(0, Array.Num() - 1);

		// Swap the two elements in the array
		int32 Temp = Array[RandIndex1];
		Array[RandIndex1] = Array[RandIndex2];
		Array[RandIndex2] = Temp;
	}
}
