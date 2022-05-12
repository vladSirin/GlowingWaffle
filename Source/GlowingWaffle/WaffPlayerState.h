// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WaffPlayerState.generated.h"

/**
 * 
 */
class AWaffPlayerState;

// Declare delegate event for health change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChanged, AWaffPlayerState*, OwningState, float,NewCreditNum, float, Delta);

UCLASS()
class GLOWINGWAFFLE_API AWaffPlayerState : public APlayerState
{
	GENERATED_BODY()


public:

	// Constructor
	AWaffPlayerState();
	
	UFUNCTION(BlueprintCallable)
	float GetPlayerCredit() const;

	UFUNCTION(BlueprintCallable)
	bool ApplyCreditChange(float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditChanged OnCreditChanged;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float Credit;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float CreditMax;

};
