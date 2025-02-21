// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffHealthPotion.h"
#include "WaffAttributeComponent.h"
#include "WaffPlayerState.h"
#include "Engine/ActorChannel.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

// Sets default values
AWaffHealthPotion::AWaffHealthPotion()
{
	HealValue = 50.0f;
	CreditCost = 5.0f;
}

void AWaffHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	AWaffPlayerState* PlayerState = Cast<AWaffPlayerState>(InstigatorPawn->GetPlayerState());
	if(PlayerState->GetPlayerCredit() < CreditCost)
	{
		UE_LOG(LogTemp, Log, TEXT("Interat Failed Not enough Credits!"))
		return;
	}
	if (InstigatorPawn)
	{
		UWaffAttributeComponent* AttriComp =  Cast<UWaffAttributeComponent>(InstigatorPawn->GetComponentByClass(UWaffAttributeComponent::StaticClass()));
		if (AttriComp && !AttriComp->IsFullHealth())
		{
			if(AttriComp->ApplyHealthChange(HealValue, InstigatorPawn))
			{
				PlayerState->ApplyCreditChange(-CreditCost);
				HideAndCoolDown();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("WaffHealthPotion, No Heal, Full Health or Null Pawn!"))
			return;
		}
	}
}

FText AWaffHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UWaffAttributeComponent* AttriComp =  Cast<UWaffAttributeComponent>(InstigatorPawn->GetComponentByClass(UWaffAttributeComponent::StaticClass()));
	if(AttriComp && AttriComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at Full Health.");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE