// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WaffSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;
	
};
