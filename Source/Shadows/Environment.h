// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "EnvironmentNode.h"
#include "Environment.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API UEnvironment : public UObject
{
	GENERATED_BODY()
	
public:
	void SetNodes(TArray<TArray<TArray<UEnvironmentNode*>>>* Nodes);

	const TArray<TArray<TArray<UEnvironmentNode*>>>& GetNodes() const;

	/**
	* Returns a list of all the open nodes that have no objects spawned in them.
	*/
	TArray<UEnvironmentNode*> GetEmptyNodes();
private:
	TArray<TArray<TArray<UEnvironmentNode*>>>* Nodes;
	
};
