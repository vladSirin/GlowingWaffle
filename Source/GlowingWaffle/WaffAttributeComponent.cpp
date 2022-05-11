// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAttributeComponent.h"

#include "GlowingWaffleGameModeBase.h"
#include "GameFramework/GameModeBase.h"

// Console variables
static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"), 1.0f, TEXT("Globale Damage Multiplier for attribute components"),
	ECVF_Cheat);


// Sets default values for this component's properties
UWaffAttributeComponent::UWaffAttributeComponent()
{
	// Initialize the values
	Health = 100;
	HealthMax = 100;
}

bool UWaffAttributeComponent::ApplyHealthChange(float Delta, AActor* Instigator)
{
	// CVar, damage multiplier applied
	if(Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	
	// Check if in god
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	// Change the health num with delta
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	// Call On Actor Killed in Game mode if health is 0.0
	if(Health == 0.0f)
	{
		AGlowingWaffleGameModeBase* GM = GetWorld()->GetAuthGameMode<AGlowingWaffleGameModeBase>();
		GM->OnActorKilled(GetOwner(), Instigator);
	}
	return ActualDelta != 0;
}

bool UWaffAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(-GetMaxHealth(), InstigatorActor);
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
	return Health / HealthMax;
}

float UWaffAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

bool UWaffAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

UWaffAttributeComponent* UWaffAttributeComponent::GetAttributes(AActor* FromActor)
{
	UWaffAttributeComponent* AttriComp = Cast<UWaffAttributeComponent>(
		FromActor->GetComponentByClass(UWaffAttributeComponent::StaticClass()));

	if (AttriComp)
	{
		return AttriComp;
	}
	return nullptr;
}

bool UWaffAttributeComponent::IsActorAlive(AActor* ActorToCheck)
{
	if (const UWaffAttributeComponent* AttributeComponent = UWaffAttributeComponent::GetAttributes(ActorToCheck))
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}
