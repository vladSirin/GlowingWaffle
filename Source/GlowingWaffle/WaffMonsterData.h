// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffAction.h"
#include "Engine/DataAsset.h"
#include "WaffMonsterData.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	/* Actions/buffs to grant this Monster */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TArray<TSubclassOf<UWaffAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
};
