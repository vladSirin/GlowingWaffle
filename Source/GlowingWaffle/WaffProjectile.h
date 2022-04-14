// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaffProjectile.generated.h"

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Setup the Necessary Components.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
