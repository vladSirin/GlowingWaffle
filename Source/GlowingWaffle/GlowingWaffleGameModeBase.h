// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WaffMonsterData.h"
#include "WaffSaveGame.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "GameFramework/GameModeBase.h"
#include "GlowingWaffleGameModeBase.generated.h"

/**
 * Define a basic Game Mode:
 * On a X time Interval, the game mode will run a Query test to find points.
 * When point is found will spawn a Minion at the location
 * Considering the Max amount of Minions on the ground, based on a float curve
 */

/* DataTable Row for spawning monsters in game mode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterID;
	// TSubclassOf<AActor> MonsterClass;

	/* Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of credits awarded to killer of this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
	
};


class UEnvQueryInstanceBlueprintWrapper;
UCLASS()
class GLOWINGWAFFLE_API AGlowingWaffleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGlowingWaffleGameModeBase();

	// Console Command
	UFUNCTION(Exec)
	void KillAll(AActor* InstigatorActor);

	// Called when any actor get killed.
	UFUNCTION()
	void OnActorKilled(AActor* Victim, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
	
protected:
	// Data table
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UDataTable* MonsterTable;
	
	// Save Game
	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	UWaffSaveGame* CurrentSaveGame;

	// Bot Spawn
	UPROPERTY()
	FTimerHandle SpawnBotTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	// UPROPERTY(EditDefaultsOnly, Category="AI")
	// TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* MaxMinionCurve;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float MinionCreditValue;

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId PrimaryAssetId, FVector SpawnLocation);
	
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(APlayerController* PlayerController);

	// Generic
	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
