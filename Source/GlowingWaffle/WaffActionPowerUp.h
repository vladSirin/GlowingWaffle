// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffAction.h"
#include "WaffPickUp.h"
#include "WaffActionPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API AWaffActionPowerUp : public AWaffPickUp
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	TSubclassOf<UWaffAction> GrantActionClass;

	
	
public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
