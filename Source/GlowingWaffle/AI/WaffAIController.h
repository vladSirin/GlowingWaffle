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

// Delegate event for when first set an actor as target.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetFirstSight, AActor*, TargetActor);

UCLASS()
class GLOWINGWAFFLE_API AWaffAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Set default
	AWaffAIController();

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();

	UPROPERTY(BlueprintAssignable)
	FOnTargetFirstSight OnTargetFirstSight;

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


