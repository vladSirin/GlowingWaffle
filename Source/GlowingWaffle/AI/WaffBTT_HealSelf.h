// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WaffBTT_HealSelf.generated.h"

/**
 * 
 */

class UWaffAttributeComponent;

UCLASS()
class GLOWINGWAFFLE_API UWaffBTT_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	float HealAmountPerTick;

	UPROPERTY(EditAnywhere, Category="AI")
	float HealRate;

	UPROPERTY()
	UWaffAttributeComponent* AttributeComponent;

	FTimerHandle HealHandler;
	FAIMessage HealFinished;
	AAIController* AIController;

	UFUNCTION()
	void HealPerTick();

	UFUNCTION()
	void FinishTask();

	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
