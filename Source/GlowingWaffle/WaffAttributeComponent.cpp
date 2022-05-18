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

	bEnableRage = false;
	Rage = 0.0f;
	RageMax = 100.0f;
}

bool UWaffAttributeComponent::ApplyHealthChange(float Delta, AActor* Instigator)
{
	// CVar, damage multiplier applied
	if (Delta < 0.0f)
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

	// Broadcast for health change delegate
	OnHealthChanged.Broadcast(Instigator, this, Health, ActualDelta);

	// Call On Actor Killed in Game mode if health is 0.0
	if (Health == 0.0f)
	{
		AGlowingWaffleGameModeBase* GM = GetWorld()->GetAuthGameMode<AGlowingWaffleGameModeBase>();
		GM->OnActorKilled(GetOwner(), Instigator);
	}
	else if (bEnableRage)
	{
		ApplyRageChange( abs(ActualDelta) * RageTransRate->GetFloatValue(abs(ActualDelta)), Instigator);
	}

	return ActualDelta != 0;
}

bool UWaffAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(-GetMaxHealth(), InstigatorActor);
}

bool UWaffAttributeComponent::ApplyRageChange(float Delta, AActor* Instigator)
{
	float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = Rage - OldRage;

	// Broadcast for rage change delegate
	OnRageChanged.Broadcast(Instigator, this, Rage, ActualDelta);

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

float UWaffAttributeComponent::GetRagePercent() const
{
	return Rage / RageMax;
}

float UWaffAttributeComponent::GetRage() const
{
	return Rage;
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
