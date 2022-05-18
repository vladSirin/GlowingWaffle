// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaffAttributeComponent.generated.h"


class UWaffAttributeComponent;

// Declare delegate event for health change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, ChangeInstigator, UWaffAttributeComponent*,
                                              OwingComp, float, NewHealth, float, Delta);

// Declare delegate event for rage change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, ChangeInstigatpr, UWaffAttributeComponent*,
                                              OwningComp,
                                              float, NewRage, float, Delta);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOWINGWAFFLE_API UWaffAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	// Sets default values for this component's properties
	UWaffAttributeComponent();

	// Static functions
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static UWaffAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(AdvancedDisplay="IsAlive"))
	static bool IsActorAlive(AActor* ActorToCheck);

	// Health change
	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(float Delta, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	// Rage Change
	UFUNCTION(BlueprintCallable)
	bool ApplyRageChange(float Delta, AActor* Instigator);

	// Const functions
	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UFUNCTION(BlueprintGetter)
	float GetHealth() const;

	UFUNCTION(BlueprintGetter)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintGetter)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintGetter)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintGetter)
	float GetRagePercent() const;

	UFUNCTION(BlueprintGetter)
	float GetRage() const;

protected:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	bool bEnableRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	float RageMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	UCurveFloat* RageTransRate;
};
