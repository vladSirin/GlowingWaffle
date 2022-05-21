// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GlowingWaffle/WaffActionComponent.h"
#include "GlowingWaffle/WaffAttributeComponent.h"
#include "GlowingWaffle/WaffWorldUserWidget.h"
#include "WaffAICharacter.generated.h"

UCLASS()
class GLOWINGWAFFLE_API AWaffAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWaffAICharacter();

protected:
	// Generic
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// UI
	UWaffWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UWaffWorldUserWidget> HealthWidgetClass;

	UWaffWorldUserWidget* AlertMark;

	UFUNCTION()
	void OnTargetFirstSight(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UWaffWorldUserWidget> AlertMarkWidgetClass;

	// Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWaffAttributeComponent* AttriComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWaffActionComponent* ActionComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent*
	                     OwingComp, float NewHealth, float Delta);

	// Effects
	UPROPERTY(VisibleAnywhere, Category="Effect")
	FName TimeToHit; // The param name of the material to trigger hit flash

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
