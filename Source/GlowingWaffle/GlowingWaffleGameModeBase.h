// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "GameFramework/GameModeBase.h"
#include "GlowingWaffleGameModeBase.generated.h"

/**
 * Define a basic Game Mode:
 * On a X time Interval, the game mode will run a Query test to find points.
 * When point is found will spawn a Minion at the location
 * Considering the Max amount of Minions on the ground, based on a float curve
 */

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
	
protected:

	// Bot Spawn
	UPROPERTY()
	FTimerHandle SpawnBotTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* MaxMinionCurve;

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(APlayerController* PlayerController);

	// Generic
	virtual void StartPlay() override;
};
