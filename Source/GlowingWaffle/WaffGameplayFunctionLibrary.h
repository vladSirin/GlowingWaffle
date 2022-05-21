// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WaffGameplayFunctionLibrary.generated.h"

/**
 * 
 */

class UWaffAction;

UCLASS()
class GLOWINGWAFFLE_API UWaffGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ConsumeRage(AActor* Instigator,AActor* TargetActor, float ConsumeAmount);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool AddAction(AActor* Instigator, TSubclassOf<UWaffAction> ActionClass);
};
