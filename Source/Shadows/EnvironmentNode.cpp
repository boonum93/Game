// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadows.h"
#include "EnvironmentNode.h"

UEnvironmentNode::UEnvironmentNode()
{
	EnvironmentActors = new TArray<FEnvironmentActorInfo>();
}

const TArray<FEnvironmentActorInfo>& UEnvironmentNode::GetEnvironmentActors() const
{
	return *EnvironmentActors;
}

void UEnvironmentNode::AddActor(FEnvironmentActorInfo ActorInfo)
{
	EnvironmentActors->Add(ActorInfo);
}

// Removes NumToRemove actors from the node that match ActorType
void UEnvironmentNode::RemoveActors(EEnvironmentActorType ActorType, uint32 NumToRemove)
{
	uint32 NumRemoved = 0;
	int32 i = 0;
	// Iterate through and remove the first instance that matches ActorType
	for (auto& ActorInfo : *EnvironmentActors)
	{
		if (ActorInfo.ActorType == ActorType)
		{
			EnvironmentActors->RemoveAt(i);
			NumRemoved++;

			if (NumRemoved == NumToRemove)
			{
				break;
			}
		}
		i++;
	}
}


