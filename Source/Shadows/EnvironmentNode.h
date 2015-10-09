// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "EnvironmentActorInfo.h"
#include "CommonEnums.h"
#include "EnvironmentNode.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWS_API UEnvironmentNode : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	* Constructor. Creates an empty EnvironmentActors array.
	*/
	UEnvironmentNode();

	/**
	* Get all of the actors located in this node.
	* @return A reference to the array of actors in this node.
	*/
	const TArray<FEnvironmentActorInfo>& GetEnvironmentActors() const;

	/**
	* Add an actor to this node with specific information given by ActorInfo.
	* @param ActorInfo Information needed for spawning the actor.
	*/
	void AddActor(FEnvironmentActorInfo ActorInfo);
	
	/**
	* Remove actors from this node according to the ActorType.
	* @param ActorType The type of actor that will be removed if in this node.
	* @param NumToRemove The number of actors of type ActorType that will be removed.
	*/
	void RemoveActors(EEnvironmentActorType ActorType, uint32 NumToRemove = 1);

private:
	// TODO: Improve performance by using a map that maps EEnvironmentActorType to arrays of that actor type.
	// RemoveActors will improve by not having to iterate through every actor in the node.
	TArray<FEnvironmentActorInfo>* EnvironmentActors; // There can be many actors in one node

};
