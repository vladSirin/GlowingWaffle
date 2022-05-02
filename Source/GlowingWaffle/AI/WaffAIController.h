// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WaffAIController.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API AWaffAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Set default
	AWaffAIController();

protected:

	UPROPERTY(EditDefaultsOnly, Category="BT")
	UBehaviorTree* BT;

	virtual void BeginPlay() override;
};


