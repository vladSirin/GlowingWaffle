// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffProjectile.h"
#include "WaffDashProjectile.generated.h"

/**
* Projectile class spawned via input key (same as MagicProjectile)
* ParticleComponent to show projectile
* ¡®Explodes¡¯ after 0.2 seconds. (Timer)
* Play particle effect at point of detonation.
* Waits 0.2 seconds again (Timer) before Teleporting PlayerCharacter (aka the ¡®Instigator¡¯ of the projectile)
* // Let the detonation effect play before we teleport
* // Make sure you ¡®stop¡¯ the projectile movement while you wait(!)
* // Look back at ¡®Instigator¡¯ we used for ignoring collisions, and re-apply here for teleportation.
* On hit with world: immediately stop movement and execute the same behavior (explode + teleport)
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API AWaffDashProjectile : public AWaffProjectile
{
	GENERATED_BODY()

public:

	AWaffDashProjectile();

protected:

	UPROPERTY()
	FTimerHandle TimHandler;

	UPROPERTY()
	FVector TeleportLocation;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplodeEmitterTemp;

	UPROPERTY(EditDefaultsOnly)
	float TimeBeforeExplode;

	UFUNCTION()
	void BeginPlay() override;

	UFUNCTION()
	void Explode();

	UFUNCTION()
	void Teleport();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};




