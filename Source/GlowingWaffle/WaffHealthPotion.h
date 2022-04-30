// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "WaffHealthPotion.generated.h"

UCLASS()
class GLOWINGWAFFLE_API AWaffHealthPotion : public AActor, public IWaffGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaffHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SMComp;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
