// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WaffCharacter.generated.h"

class UWaffAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class UWaffInteractionComponent;
class AWaffProjectile;

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

	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.

	// Camera and SpringArm
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// Attack and Abilities
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AWaffProjectile> PrimaryProjectile;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AWaffProjectile> SecondaryProjectile;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AWaffProjectile> DashProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, Category="Effect")
	FName HandSocketName; 	// HandSocketName used to spawn effects and projectile

	UPROPERTY(VisibleAnywhere, Category="Effect")
	FName TimeToHit; // The name of Material param for on hit flash
	
	UFUNCTION(BlueprintCallable)
	void PrimaryAttack();

	UFUNCTION(BlueprintCallable)
	void SecondaryAttack();

	UFUNCTION(BlueprintCallable)
	void DashCast();

	UFUNCTION()
	void Attack_TimeElapsed(TSubclassOf<AWaffProjectile> AttackProjectile);
	
	UPROPERTY() // Ref: https://www.tomlooman.com/unreal-engine-cpp-timers/
	FTimerHandle AttackTimerHandler; 	// Timer handler and Delegate 

	FTimerDelegate AttackTimerDelegate; 	// Timer Delegate to bind function with var to be used along with Timer handler


	// Interactions
	UPROPERTY(VisibleAnywhere)
	UWaffInteractionComponent* InteractComp;

	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	// Movement functions
	void MoveForward(float value);

	void MoveRight(float value);

	// Attributes related
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWaffAttributeComponent* AttriComp;

	UFUNCTION()
	void OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent*
	                     OwingComp, float NewHealth, float Delta);

	// Post initialize
	UFUNCTION()
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
