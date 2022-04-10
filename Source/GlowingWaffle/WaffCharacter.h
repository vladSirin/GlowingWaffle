// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WaffCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

	// TODO: Define a Projectile class property
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> MagicProjectile;

	void MoveForward(float value);

	void MoveRight(float value);

	// TODO: Setup primary attack. using the hand socket
	UFUNCTION(BlueprintCallable)
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
