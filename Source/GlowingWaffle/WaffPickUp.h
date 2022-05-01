// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "WaffPickUp.generated.h"

/*
* This is a common Pickup Class used for different pick-up power-up items.
* React to Interact() // SGameplayInterface.h
* Inactivate for X seconds upon triggered (Invisible and unable to interact with)
* Re-activates on time elapsed to show mesh, and ready to be triggered again.
 */

UCLASS()
class GLOWINGWAFFLE_API AWaffPickUp : public AActor, public IWaffGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaffPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SMComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	float Cooldown;

	FTimerHandle CooldownHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interact Implementation
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	// Disable health potion
	void Disable();

	// Enable health potion
	void Enable();
};
