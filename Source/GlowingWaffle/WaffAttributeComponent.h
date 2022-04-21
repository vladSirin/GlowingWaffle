// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaffAttributeComponent.generated.h"


class UWaffAttributeComponent;

// Declare delegate event for health change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, ChangeInstigator, UWaffAttributeComponent*,
                                              OwingComp, float, NewHealth, float, Delta);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOWINGWAFFLE_API UWaffAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaffAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealthMax;

public:
	UFUNCTION(BlueprintCallable)
	bool OnHealthChange(float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
