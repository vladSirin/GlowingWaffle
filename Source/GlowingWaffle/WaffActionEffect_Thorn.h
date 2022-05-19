// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffActionEffect.h"
#include "WaffActionEffect_Thorn.generated.h"

/**
 * 
 */

class UWaffAttributeComponent;

UCLASS()
class GLOWINGWAFFLE_API UWaffActionEffect_Thorn : public UWaffActionEffect
{
	GENERATED_BODY()

public:
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void ThornOnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent*
	                          OwingComp, float NewHealth, float Delta);

	UPROPERTY(EditDefaultsOnly, Category="Thorn")
	float ThornRate;
};
