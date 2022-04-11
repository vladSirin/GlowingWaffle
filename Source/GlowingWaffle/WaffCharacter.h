// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WaffCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWaffInteractionComponent;

UCLASS()
class GLOWINGWAFFLE_API AWaffCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWaffCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// Define a Projectile class property and attack anim montage
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AActor> MagicProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY()
	FTimerHandle AttackTimerHandler;

	UPROPERTY(VisibleAnywhere)
	UWaffInteractionComponent* InteractComp;

	void MoveForward(float value);

	void MoveRight(float value);

	// Setup primary attack. using the hand socket
	UFUNCTION(BlueprintCallable)
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
