// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAction.h"

#include "Engine/ActorChannel.h"

void UWaffAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UWaffAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* UWaffAction::GetWorld() const
{
	// Outer is set when creating action object via New object<t>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
