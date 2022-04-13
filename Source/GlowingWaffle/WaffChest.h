// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaffGameplayInterface.h"
#include "WaffChest.generated.h"

UCLASS()
class GLOWINGWAFFLE_API AWaffChest : public AActor, public IWaffGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaffChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Static Mesh comps
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ChestBottmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ChestLidComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn);

	UPROPERTY(EditAnywhere)
	float TargetPitch;

};
