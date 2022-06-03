// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WaffPlayerState.generated.h"

/**
 * 
 */

// Using a struct to make sure player credits change are synced
USTRUCT(BlueprintType)
struct FPlayerCredit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	float CreditNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	float LastDelta;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	float CreditMax;
};

class AWaffPlayerState;

// Declare delegate event for health change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChanged, AWaffPlayerState*, OwningState, float, NewCreditNum,
                                               float, Delta);

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
	// Using a struct to make sure player credits change are synced
	UPROPERTY(ReplicatedUsing="OnRep_Credit", EditDefaultsOnly, Category="Gameplay")
	FPlayerCredit PlayerCredit;

	// [Networking]
	UFUNCTION()
	void OnRep_Credit();
};
