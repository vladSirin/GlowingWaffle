// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "WaffPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API AWaffPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AWaffPlayerController();
	
protected:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChange;

	virtual void SetPawn(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Faction")
	FGenericTeamId TeamId;

	UFUNCTION(BlueprintCallable)
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void PostInitializeComponents() override;
	
};
