// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaffExplosiveBarrel.generated.h"

class URadialForceComponent;
class USphereComponent;

UCLASS()
class GLOWINGWAFFLE_API AWaffExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaffExplosiveBarrel();

protected:

	// Components
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* SMComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleAnywhere, Category="Componenet")
	URadialForceComponent* ForceComp;

public:	

	// Called before beginplay but after constructors
	virtual void PostInitializeComponents() override;

	// Explore function
	UFUNCTION(BlueprintCallable)
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void Explode();

};
