// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAction.h"

#include "GlowingWaffle.h"
#include "Net/UnrealNetwork.h"

void UWaffAction::Initialize(UWaffActionComponent* ActionComponent)
{
	OwningComponent = ActionComponent;
}

void UWaffAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	// Set the TimeStarted, only runs on Authority and replicate this to clients
	if(GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	//Log on screen
	// LogOnScreen(this, FString::Printf(TEXT("%s Action Start"), *ActonName.ToString()), FColor::Green);

	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.AppendTags(TagsToGrant);
	RepData.bRunning = true;
	RepData.Instigator = Instigator;

	Comp->OnActionStarted.Broadcast(Comp, this);
}

void UWaffAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	//Log on screen
	// LogOnScreen(this, FString::Printf(TEXT("%s Action Stop"), *ActonName.ToString()), FColor::Green);

	// ensureAlways(bRunning); not longer necessary as now server decides on this.

	UWaffActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.RemoveTags(TagsToGrant);
	RepData.bRunning = false;
	RepData.Instigator = Instigator;

	Comp->OnActionStopped.Broadcast(Comp, this);
}

bool UWaffAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bRunning)
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
	return RepData.bRunning;
}

/* [Networking]
 * Network related functions
 * Action is a UObject child class
 */

// RepNotify for bRunning
void UWaffAction::OnRep_RepData()
{
	if (RepData.bRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


// Prop replication function
void UWaffAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWaffAction, RepData);
	DOREPLIFETIME(UWaffAction, OwningComponent);
	DOREPLIFETIME(UWaffAction, TimeStarted);
}
