// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "WaffBTS_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffBTS_CheckHealth : public UBTService
{
	GENERATED_BODY()

protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector bLowHeathKey;

	UPROPERTY(EditAnywhere, Category="AI")
	float LowHealthThreshold;
};
