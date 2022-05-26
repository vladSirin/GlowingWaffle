// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaffAttributeComponent.generated.h"


class UWaffAttributeComponent;

// // Declare delegate event for health change
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, ChangeInstigator, UWaffAttributeComponent*,
//                                               OwningComp, float, NewHealth, float, Delta);
// // Declare delegate event for rage change
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, ChangeInstigatpr, UWaffAttributeComponent*,
//                                               OwningComp,
//                                               float, NewRage, float, Delta);

// Declare a delegate general event for both rage and health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, ChangeInstigator, UWaffAttributeComponent*,
                                              OwningComp, float, NewValue, float, Delta);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOWINGWAFFLE_API UWaffAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Delegate
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

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
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly)
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	bool bEnableRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	float RageMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Rage")
	UCurveFloat* RageTransRate;

	// Multicast or onHealthChange, Multicast is ideal for events, while state change should be handled by RepNotify
	// State changes can also easily used for sync between different players, like joined later
	// Reliable multicasts are always considered with Relevancy, which means all the clients on server will get notified, good for message
	UFUNCTION(NetMulticast, Reliable)
	//@FIXME: Mark as unreliable once we moved 'state' change out of the character class
	void MulticastOnHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
};
