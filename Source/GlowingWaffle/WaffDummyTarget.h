// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "WaffDummyTarget.generated.h"

UCLASS()
class GLOWINGWAFFLE_API AWaffDummyTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaffDummyTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* SMComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UWaffAttributeComponent* AttriComp;

	// On Health Change func for bind
	UFUNCTION()
	void OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent*
	                     OwingComp, float NewHealth, float Delta);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
