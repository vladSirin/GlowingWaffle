// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffAction.h"
#include "WaffActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffActionEffect : public UWaffAction
{
	GENERATED_BODY()

public:
	UWaffActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;


	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="Effect")
	float Period;

protected:
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category="Effect")
	void ExecutePeriodEffect(AActor* Instigator);
};
