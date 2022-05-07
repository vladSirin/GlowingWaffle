// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WaffBTT_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffBTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UWaffBTT_RangedAttack();
	
protected:
	UPROPERTY(EditAnywhere, Category="BTT")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="BTT")
	float MaxBulletSpread;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
