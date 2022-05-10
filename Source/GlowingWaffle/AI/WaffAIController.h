// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISense_Sight.h"
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

	UFUNCTION(BlueprintSetter)
	void SetTargetActor(AActor* InstigatorActor);

	UFUNCTION(BlueprintGetter)
	AActor* GetTargetActor();

protected:

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BT;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	FGenericTeamId TeamId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	FName TargetActorKey;
	
	UFUNCTION(BlueprintCallable)
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};


