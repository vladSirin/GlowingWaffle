// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaffAction.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class GLOWINGWAFFLE_API UWaffAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	
	/* Action Nick Name to start/stop an action without referencing */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActonName;
	
	virtual UWorld* GetWorld() const override;
	
};
