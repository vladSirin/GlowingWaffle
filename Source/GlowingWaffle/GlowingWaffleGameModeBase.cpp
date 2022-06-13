// Copyright Epic Games, Inc. All Rights Reserved.


#include "GlowingWaffleGameModeBase.h"
#include "EngineUtils.h"
#include "WaffCharacter.h"
#include "WaffPlayerController.h"
#include "WaffPlayerState.h"
#include "AI/WaffAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

// Console variables
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of Bots via timer"),
                                                ECVF_Cheat);

AGlowingWaffleGameModeBase::AGlowingWaffleGameModeBase()
{
	SpawnInterval = 2.0f;
	MinionCreditValue = 1.0f;

	SlotName = "SaveGame01";
}



void AGlowingWaffleGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continue bot spawning based on timer
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AGlowingWaffleGameModeBase::SpawnBotTimerElapsed,
	                                SpawnInterval, true);

	// Make sure necessary items are set
	ensure(MinionClass);
	ensure(SpawnBotQuery);
	ensure(MaxMinionCurve);
}

void AGlowingWaffleGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void AGlowingWaffleGameModeBase::SpawnBotTimerElapsed()
{
	// Checking console var
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		// Output log so we know it is because of Cvar
		UE_LOG(LogTemp, Warning, TEXT("Bots spawning disabled via Cvar 'CVarSpawnBots'"),);
		return;
	}

	float CurrentAliveMinions = 0;

	// this is the C++ version of get actor of class in level, and iterate on this
	// Iterate in level to find the alive bots, if too many dont spawn.
	for (TActorIterator<AWaffAICharacter> ItActor(GetWorld()); ItActor; ++ItActor)
	{
		UActorComponent* AttriComp = UWaffAttributeComponent::GetAttributes(*ItActor);
		if (ensure(AttriComp) && UWaffAttributeComponent::IsActorAlive(*ItActor))
		{
			CurrentAliveMinions++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), CurrentAliveMinions);

	if (ensure(MaxMinionCurve))
	{
		if (MaxMinionCurve->GetFloatValue(GetWorld()->TimeSeconds) <= CurrentAliveMinions)
		{
			UE_LOG(LogTemp, Log, TEXT("Max Minion Count Reached!"))
			return;
		}
	}

	// Start Spawn position query
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		GetWorld(), SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGlowingWaffleGameModeBase::OnQueryFinished);
}

// Spawn bot at location when query finished
void AGlowingWaffleGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                 EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Query Failed, cannot spawn bot!"))
		return;
	}
	TArray<FVector> QueryLocationArray;
	QueryInstance->GetQueryResultsAsLocations(QueryLocationArray);

	if (!QueryLocationArray.IsEmpty())
	{
		AActor* SpawnedMinion = GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocationArray[0],
		                                                       FRotator::ZeroRotator);

		// Debug position of spawning
		DrawDebugSphere(GetWorld(), QueryLocationArray[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}


// Handle when actor is killed in game
void AGlowingWaffleGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	if (AWaffCharacter* Character = Cast<AWaffCharacter>(Victim))
	{
		FTimerHandle RespawnDelay;

		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, "RespawnPlayerElapsed", Character->GetController());

		GetWorldTimerManager().SetTimer(RespawnDelay, RespawnDelegate, 5.0f, false);
	}
	else if (APawn* KillerPawn = Cast<APawn>(Killer))
	{
		if (AWaffPlayerState* PS = KillerPawn->GetPlayerState<AWaffPlayerState>())
		{
			PS->ApplyCreditChange(MinionCreditValue);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("On Actor Killed: Victim %s, Killer %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}


// respawn player by controller.
void AGlowingWaffleGameModeBase::RespawnPlayerElapsed(APlayerController* PlayerController)
{
	if (ensure(PlayerController))
	{
		PlayerController->UnPossess();

		RestartPlayer(PlayerController);
	}
}

// Console command: killAll
void AGlowingWaffleGameModeBase::KillAll(AActor* InstigatorActor)
{
	for (TActorIterator<AWaffAICharacter> ItActor(GetWorld()); ItActor; ++ItActor)
	{
		UActorComponent* AttriComp = UWaffAttributeComponent::GetAttributes(*ItActor);
		if (ensure(AttriComp) && UWaffAttributeComponent::IsActorAlive(*ItActor))
		{
			Cast<UWaffAttributeComponent>(AttriComp)->Kill(this); //@ToDo, Pass in player later
		}
	}
}
void AGlowingWaffleGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AGlowingWaffleGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UWaffSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Load Save Data!"));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."))
	}
	else
	{
		CurrentSaveGame = Cast<UWaffSaveGame>(UGameplayStatics::CreateSaveGameObject(UWaffSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data!"))
	}
}
