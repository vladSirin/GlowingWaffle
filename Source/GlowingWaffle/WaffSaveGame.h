// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WaffSaveGame.generated.h"

/* This is a struct build to save all related actors in game
 * The name and the transform of the actors will be saved and loaded based on different needs
 */
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which actor this belongs to */
	UPROPERTY()
	FString ActorName;

	/* For Movable Actors, keep location, rotation and scale */
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class GLOWINGWAFFLE_API UWaffSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
