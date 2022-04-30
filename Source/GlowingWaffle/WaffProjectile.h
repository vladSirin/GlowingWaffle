// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaffProjectile.generated.h"

/*
 * This is the Projectile base class, it handles:
 * The basic components of projectile,
 * The basic overlap function
 * The basic OnHit
 * the basic explode, using when OnHit
 */

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GLOWINGWAFFLE_API AWaffProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaffProjectile();

protected:
	// Damage Value, minus to health
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage;

	// Audio
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;

	// VFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ImpactVFX;

	// Basic Comps for movement, particle and collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComp;

	/*
	 * Behavior when Overlap with another Other
	 * The Original Implementation will Explode and Destroy the projectile itself.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	 * Behavior when Overlap with another Other
	 * The Original Implementation will Explode and Destroy the projectile itself.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

	/*
	 * Explode will play the impact VFX and Sound, stop the movement and collision, deactivate any visual effect
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

public:
	virtual void PostInitializeComponents() override;
};
