// Copyright Epic Games, Inc. All Rights Reserved.


#include "GlowingWaffleGameModeBase.h"
#include "EngineUtils.h"
#include "AI/WaffAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AGlowingWaffleGameModeBase::AGlowingWaffleGameModeBase()
{
	SpawnInterval = 2.0f;
}

void AGlowingWaffleGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continue bot spawning based on timer
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AGlowingWaffleGameModeBase::SpawnBotTimerElapsed, SpawnInterval, true);

	// Make sure necessary items are set
	ensure(MinionClass);
	ensure(SpawnBotQuery);
	ensure(MaxMinionCurve);
}

void AGlowingWaffleGameModeBase::SpawnBotTimerElapsed()
{
	float CurrentAliveMinions = 0;

	// this is the C++ version of get actor of class in level, and iterate on this
	// Iterate in level to find the alive bots, if too many dont spawn.
	for(TActorIterator<AWaffAICharacter> ItActor(GetWorld()); ItActor; ++ItActor)
	{
		UActorComponent* AttriComp = UWaffAttributeComponent::GetAttributes(*ItActor);
		if(ensure(AttriComp) && UWaffAttributeComponent::IsActorAlive(*ItActor))
		{
			CurrentAliveMinions++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), CurrentAliveMinions);

	if(ensure(MaxMinionCurve))
	{
		if(MaxMinionCurve->GetFloatValue(GetWorld()->TimeSeconds) <= CurrentAliveMinions)
		{
			UE_LOG(LogTemp, Log, TEXT("Max Minion Count Reached!"))
			return;
		}
	}

	// Start Spawn position query
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGlowingWaffleGameModeBase::OnQueryFinished);


}

void AGlowingWaffleGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Query Failed, cannot spawn bot!"))
		return;
	}
	TArray<FVector> QueryLocationArray;
	QueryInstance->GetQueryResultsAsLocations(QueryLocationArray);

	if(!QueryLocationArray.IsEmpty())
	{
		AActor* SpawnedMinion = GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocationArray[0], FRotator::ZeroRotator);

		// Debug position of spawning
		DrawDebugSphere(GetWorld(), QueryLocationArray[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}
