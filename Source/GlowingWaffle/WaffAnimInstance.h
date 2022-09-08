// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffActionComponent.h"
#include "Animation/AnimInstance.h"
#include "WaffAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	/* Is Pawn Stunned based on gameplayTag Data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	UWaffActionComponent* ActionComp;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
