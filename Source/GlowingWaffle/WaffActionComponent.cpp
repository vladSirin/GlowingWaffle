// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffActionComponent.h"

#include "GlowingWaffle.h"
#include "WaffAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UWaffActionComponent::UWaffActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}

UWaffActionComponent* UWaffActionComponent::GetActionComponent(AActor* FromActor)
{
	UWaffActionComponent* Comp = Cast<UWaffActionComponent>(
		FromActor->GetComponentByClass(UWaffActionComponent::StaticClass()));
	if (Comp)
	{
		return Comp;
	}
	return nullptr;
}

bool UWaffActionComponent::IsActionExist(AActor* FromActor, TSubclassOf<UWaffAction> ActionClass)
{
	UWaffAction** Action = GetActionComponent(FromActor)->ActionList.FindByPredicate
	(
		[&](const UWaffAction* WaffAction)
		{
			return WaffAction->GetClass() == ActionClass;
		}
	);
	if (Action)
	{
		return true;
	}
	return false;
}


// Called when the game starts
void UWaffActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// [Networking]
	// Server only, we only want the actions instantiated once on server.
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UWaffAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}


// Called every frame
void UWaffActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTag.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Log on screen to debug server client
	for (UWaffAction* Action : ActionList)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString Msg;
		Msg = FString::Printf(
			TEXT("[%s] Action: %s : IsRunning: %s : Outer %s"), *GetNameSafe(GetOwner()), *Action->ActonName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"), *GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, Msg, TextColor, 0.0f);
	}
}



bool UWaffActionComponent::AddAction(AActor* Instigator, TSubclassOf<UWaffAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return false;
	}
	UWaffAction* NewAction = NewObject<UWaffAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		ActionList.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

void UWaffActionComponent::RemoveAction(UWaffAction* Action)
{
	if (!ensure(Action || !Action->IsRunning()))
	{
		return;
	}
	ActionList.Remove(Action);
}

bool UWaffActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UWaffAction* Action : ActionList)
	{
		if (Action && !Action->CanStart(Instigator))
		{
			FString FailMsg = FString::Printf(TEXT("Fail to run: %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailMsg);
			continue;
		}

		if (Action && Action->ActonName == ActionName)
		{
			// If not server, call server function
			if (!GetOwner()->HasAuthority())
			{
				Server_StartAction(Instigator, ActionName);
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}


bool UWaffActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UWaffAction* Action : ActionList)
	{
		if (!Action->IsRunning())
		{
			continue;
		}
		if (Action && Action->ActonName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

/*
 * Network related
 */

void UWaffActionComponent::Server_StartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UWaffActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWaffActionComponent, ActionList);
}


// Using the actor channel to replicate he UObjects subclass - Actions
bool UWaffActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomthing = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(UWaffAction* Action : ActionList)
	{
		if(Action)
		{
			WroteSomthing |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomthing;
}
