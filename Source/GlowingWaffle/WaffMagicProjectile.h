// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaffMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class GLOWINGWAFFLE_API AWaffMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaffMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TODO: Setup the Necessary Components.
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComp;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
