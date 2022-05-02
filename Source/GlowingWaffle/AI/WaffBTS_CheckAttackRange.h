// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "WaffBTS_CheckAttackRange.generated.h"

/** 
 * This is the BTS used to check range and LOS for AI
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffBTS_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector AttackRangeKey;
};
