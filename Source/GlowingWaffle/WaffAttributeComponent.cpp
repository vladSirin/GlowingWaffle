// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAttributeComponent.h"

// Sets default values for this component's properties
UWaffAttributeComponent::UWaffAttributeComponent()
{
	// Initialize the values
	Health = 100;
	HealthMax = 100;
}


// Called when the game starts
void UWaffAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


bool UWaffAttributeComponent::ApplyHealthChange(float Delta)
{
	// Change the health num with delta
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	return ActualDelta != 0;
}

bool UWaffAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UWaffAttributeComponent::GetHealth() const
{
	return Health;
}

float UWaffAttributeComponent::GetHealthPercent() const
{
	return Health/HealthMax;
}

bool UWaffAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

UWaffAttributeComponent* UWaffAttributeComponent::GetAttributes(AActor* FromActor)
{
	UWaffAttributeComponent* AttriComp = Cast<UWaffAttributeComponent>(FromActor->GetComponentByClass(UWaffAttributeComponent::StaticClass()));
	
	if (AttriComp)
	{
		return AttriComp;
	}
	return nullptr;
}

bool UWaffAttributeComponent::IsActorAlive(AActor* ActorToCheck)
{
	if(const UWaffAttributeComponent* AttributeComponent = UWaffAttributeComponent::GetAttributes(ActorToCheck))
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}
