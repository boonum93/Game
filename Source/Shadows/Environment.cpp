// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "Environment.h"

void UEnvironment::SetNodes(TArray<TArray<TArray<UEnvironmentNode*>>>* Nodes)
{
	this->Nodes = Nodes;
}

const TArray<TArray<TArray<UEnvironmentNode*>>>& UEnvironment::GetNodes() const
{
	return *Nodes;
}


