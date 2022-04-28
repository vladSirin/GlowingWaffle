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
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

bool UWaffAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
