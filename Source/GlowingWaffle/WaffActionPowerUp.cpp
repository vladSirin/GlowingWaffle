// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffActionPowerUp.h"

#include "WaffGameplayFunctionLibrary.h"

void AWaffActionPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if(GrantActionClass && !UWaffActionComponent::IsActionExist(InstigatorPawn, GrantActionClass))
	{
		UWaffGameplayFunctionLibrary::AddAction(InstigatorPawn, GrantActionClass);
		HideAndCoolDown();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ActionPowerUp: Add Action Failed!"));
}
