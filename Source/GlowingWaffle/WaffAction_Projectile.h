// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffAction.h"
#include "WaffAction_Projectile.generated.h"

/**
 * 
 */
class AWaffProjectile;

UCLASS()
class GLOWINGWAFFLE_API UWaffAction_Projectile : public UWaffAction
{
	GENERATED_BODY()

public:
	UWaffAction_Projectile();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<AWaffProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	FName HandSocketName; 	// HandSocketName used to spawn effects and projectile

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackDelay;

	UPROPERTY() // Ref: https://www.tomlooman.com/unreal-engine-cpp-timers/
	FTimerHandle AttackTimerHandler; // Timer handler and Delegate 

	FTimerDelegate AttackTimerDelegate; // Timer Delegate to bind function with var to be used along with Timer handler

	UFUNCTION()
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	UFUNCTION()
	void Attack_TimeElapsed(TSubclassOf<AWaffProjectile> AttackProjectile, AActor* Instigator);
};
