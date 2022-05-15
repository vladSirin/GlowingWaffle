// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAction.h"

#include "Engine/ActorChannel.h"

void UWaffAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.AppendTags(TagsToGrant);
	bRunning = true;
}

void UWaffAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bRunning);
	
	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.RemoveTags(TagsToGrant);
	bRunning = false;
}

bool UWaffAction::CanStart_Implementation(AActor* Instigator)
{
	if(bRunning)
	{
		return false;
	}
	
	UWaffActionComponent* Comp = GetOwningComponent();
	if(Comp->ActiveGameplayTag.HasAny(BlockingTags))
	{
		return false;
	}
	return true;

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

UWaffActionComponent* UWaffAction::GetOwningComponent() const
{
	return Cast<UWaffActionComponent>(GetOuter());
}

bool UWaffAction::IsRunning() const
{
	return bRunning;
}
