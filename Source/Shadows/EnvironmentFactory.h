// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Environment.h"
#include "EnvironmentFactory.generated.h"

/**
* This class is an interface for factory classes that construct
* Environments. Each subclass implements a MakeEnvironment() function
* that generates an environment based off of a unique generation algorithm.
*/
UCLASS()
class SHADOWS_API UEnvironmentFactory : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UEnvironmentFactory() {}
	/**
	* Creates an environment object.
	*/
	virtual UEnvironment* MakeEnvironment(int32 NumStartingPoints, int32 Length, int32 Width, int32 Floors)
	{
		check(0 && "You must override this");
		return nullptr;
	}

};