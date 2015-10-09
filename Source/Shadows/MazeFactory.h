// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentFactory.h"
#include "Environment.h"
#include "MazeFactory.generated.h"

/**
*
*/
UCLASS()
class SHADOWS_API UMazeFactory : public UEnvironmentFactory
{
	GENERATED_BODY()

public:
	virtual ~UMazeFactory() {}
	virtual UEnvironment* MakeEnvironment(int32 NumStartingPoints, int32 Length, int32 Width, int32 Floors) override;

private:
	void MakeSet(TArray<int32>& Parent, TArray<int32>& Rank, int32 ArrayLength);
	int32 FindSet(TArray<int32>& Parent, int32 X);
	void Union(TArray<int32>& Parent, TArray<int32> Rank, int32 X, int32 Y);
	void ShuffleArray(TArray<int32>& Array);
};

