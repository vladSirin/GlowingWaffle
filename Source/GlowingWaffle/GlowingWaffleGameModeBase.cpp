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
}

void AGlowingWaffleGameModeBase::SpawnBotTimerElapsed()
{
	if(ensure(SpawnBotQuery))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGlowingWaffleGameModeBase::OnQueryFinished);
	}

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

	if(ensure(MinionClass))
	{
		float CurrentAliveMinions = 0;
		for(TActorIterator<AActor> ItActor = TActorIterator<AActor>(GetWorld(), MinionClass); ItActor; ++ItActor)
		{
			CurrentAliveMinions++;
		}

		if(ensure(MaxMinionCurve))
		{
			if(MaxMinionCurve->GetFloatValue(GetWorld()->TimeSeconds) <= CurrentAliveMinions)
			{
				UE_LOG(LogTemp, Log, TEXT("Max Minion Count Reached!"))
				return;
			}
		}
	
		if(!QueryLocationArray.IsEmpty())
		{
			AActor* SpawnedMinion = GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocationArray[0], FRotator::ZeroRotator);
		}

	}
}
