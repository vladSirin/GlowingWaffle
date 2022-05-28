// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAction.h"

#include "GlowingWaffle.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UWaffAction::Initialize(UWaffActionComponent* ActionComponent)
{
	OwningComponent = ActionComponent;
}
void UWaffAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	//Log on screen
	LogOnScreen(this, FString::Printf(TEXT("%s Action Start"), *ActonName.ToString()), FColor::Green);

	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.AppendTags(TagsToGrant);
	bRunning = true;
}

void UWaffAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	//Log on screen
	LogOnScreen(this, FString::Printf(TEXT("%s Action Stop"), *ActonName.ToString()), FColor::Green);

	// ensureAlways(bRunning); not longer necessary as now server decides on this.

	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.RemoveTags(TagsToGrant);
	bRunning = false;
}

bool UWaffAction::CanStart_Implementation(AActor* Instigator)
{
	if (bRunning)
	{
		return false;
	}

	UWaffActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTag.HasAny(BlockingTags))
	{
		return false;
	}
	return true;
}


UWorld* UWaffAction::GetWorld() const
{
	// Outer is set when creating action object via New object<t>
	// Using Actor Instead of an ActionComponent cause when replicating SubObject, Actor is only allowed.
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

UWaffActionComponent* UWaffAction::GetOwningComponent() const
{
	return OwningComponent;
}

bool UWaffAction::IsRunning() const
{
	return bRunning;
}

/* [Networking]
 * Network related functions
 * Action is a UObject child class
 */

// RepNotify for bRunning
void UWaffAction::OnRep_IsRunning()
{
	if (bRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}



// Prop replication function
void UWaffAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWaffAction, bRunning);
	DOREPLIFETIME(UWaffAction, OwningComponent);
}
