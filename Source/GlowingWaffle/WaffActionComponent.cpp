// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffActionComponent.h"

#include "WaffAction.h"


// Sets default values for this component's properties
UWaffActionComponent::UWaffActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	UWaffAction** Action = GetActionComponent(FromActor)->Actions.FindByPredicate
	(
		[&](const UWaffAction* WaffAction)
		{
			return WaffAction->GetClass() == ActionClass;
		}
	);
	if(Action)
	{
		return true;
	}
	return false;
}


// Called when the game starts
void UWaffActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultActions.IsEmpty())
	{
		for (TSubclassOf<UWaffAction> ActionClass : DefaultActions)
		{
			if(ActionClass)
			{
				AddAction(GetOwner(), ActionClass);
			}
		}
	}
}


// Called every frame
void UWaffActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTag.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// ...
}

bool UWaffActionComponent::AddAction(AActor* Instigator, TSubclassOf<UWaffAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return false;
	}
	UWaffAction* NewAction = NewObject<UWaffAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
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
	Actions.Remove(Action);
}

bool UWaffActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UWaffAction* Action : Actions)
	{
		if (Action && !Action->CanStart(Instigator))
		{
			FString FailMsg = FString::Printf(TEXT("Fail to run: %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailMsg);
			continue;
		}

		if (Action && Action->ActonName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UWaffActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UWaffAction* Action : Actions)
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
