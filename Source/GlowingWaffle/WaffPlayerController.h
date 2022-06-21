// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "WaffPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

	virtual void SetPawn(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Faction")
	FGenericTeamId TeamId;

	UFUNCTION(BlueprintCallable)
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void PostInitializeComponents() override;

	virtual void OnRep_PlayerState() override;

	/* Called when player controller is ready to begin playing, good moment to initialize things like UI which might be to early in BeginPlay
	 * (esp. in multiplayer clients where not all data such as PlayerState may have been received yet
	 */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	
};
