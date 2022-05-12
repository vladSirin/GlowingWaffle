// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCoinPickup.h"

#include "WaffPlayerState.h"

AWaffCoinPickup::AWaffCoinPickup()
{
	CoinPicked = 5.0f;
}

void AWaffCoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(InstigatorPawn)
	{
		AWaffPlayerState* PlayerState = InstigatorPawn->GetController()->GetPlayerState<AWaffPlayerState>();
		if(PlayerState)
		{
			PlayerState->ApplyCreditChange(CoinPicked);
			HideAndCoolDown();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CoinPickUp Failed, No Proper PlayerState!"))
		}
	}
}
