// Copyright Epic Games, Inc. All Rights Reserved.


#include "GlowingWaffleGameModeBase.h"
#include "EngineUtils.h"
#include "WaffCharacter.h"
#include "WaffGameplayInterface.h"
#include "WaffPlayerController.h"
#include "WaffPlayerState.h"
#include "AI/WaffAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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

void AGlowingWaffleGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AWaffPlayerState* PS = NewPlayer->GetPlayerState<AWaffPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AGlowingWaffleGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame(); //@todo: putting this here so it works, InitGame is too early for manipulating actors
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
	//Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AWaffPlayerState* PS = Cast<AWaffPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; //@todo: Single player only now
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors and save the relevant ones
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our "Gameplay Actors"
		if (!Actor->Implements<UWaffGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;

		// Coverts Actor's SaveGame UPROPERTIES into binary way;
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
		UE_LOG(LogTemp, Log, TEXT("Writing SaveGame, Saved %s, current Saved Actor Number: %i"), *ActorData.ActorName,
		       CurrentSaveGame->SavedActors.Num());
	}

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

		// Iterate the entire world of actors and save the relevant ones
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our "Gameplay Actors"
			if (!Actor->Implements<UWaffGameplayInterface>()) //Using Prefix U on Casting to Interfaces
			{
				continue;
			}

			// If the Actor is relevant, go trough the saved names and set transform if found
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					UE_LOG(LogTemp, Log, TEXT("Loading SaveGame, Loaded %s"), *ActorData.ActorName);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

					// Find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;

					// Coverts binary array into actor's varibales
					Actor->Serialize(Ar);

					IWaffGameplayInterface::Execute_OnActorLoaded(Actor); //using Prefix I on calling static functions

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UWaffSaveGame>(UGameplayStatics::CreateSaveGameObject(UWaffSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data!"))
	}
}
