// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffActionEffect_Thorn.h"
#include "WaffAttributeComponent.h"
#include "GameFramework/Character.h"
#include "WaffGameplayFunctionLibrary.h"

void UWaffActionEffect_Thorn::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UWaffAttributeComponent* Comp = UWaffAttributeComponent::GetAttributes(Instigator);
	if (ensure(Comp))
	{
		Comp->OnHealthChanged.AddDynamic(this, &UWaffActionEffect_Thorn::ThornOnHealthChanged);
	}
}

void UWaffActionEffect_Thorn::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	UWaffAttributeComponent* Comp = UWaffAttributeComponent::GetAttributes(Instigator);
	if (ensure(Comp))
	{
		Comp->OnHealthChanged.RemoveDynamic(this, &UWaffActionEffect_Thorn::ThornOnHealthChanged);
	}
}

void UWaffActionEffect_Thorn::ThornOnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent* OwingComp,
                                                   float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	if (ChangeInstigator && Delta < 0.0f)
	{
		if (ChangeInstigator == OwningActor)
		{
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Thorn in Effect, Return %f Damage to Instigator!"), - Delta*ThornRate);
		UWaffGameplayFunctionLibrary::ApplyDamage(OwningActor, ChangeInstigator,
		                                          FMath::RoundToInt(- Delta * ThornRate));
	}
}
