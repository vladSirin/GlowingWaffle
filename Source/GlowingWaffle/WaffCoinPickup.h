// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffPickUp.h"
#include "WaffCoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API AWaffCoinPickup : public AWaffPickUp
{
	GENERATED_BODY()

public:
	// Constructor
	AWaffCoinPickup();

protected:
	// Coin Get on Pickup
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float CoinPicked;

	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
