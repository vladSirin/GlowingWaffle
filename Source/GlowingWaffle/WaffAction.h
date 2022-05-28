// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WaffActionComponent.h"
#include "UObject/NoExportTypes.h"
#include "WaffAction.generated.h"

class UWorld;
/**
 * WaffAction class is a child class of UObject, which means it does not replicate lik Actors and Components
 */
UCLASS(Blueprintable)
class GLOWINGWAFFLE_API UWaffAction : public UObject
{
	GENERATED_BODY()

protected:
	/* Tags added to owning actor when activated, removed when stopped */
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer TagsToGrant;

	/* Actions can only start when actor has none of the Tags applied */
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockingTags;

	/*
	 * [Networking]
	 */
	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	bool bRunning;

	UFUNCTION()
	void OnRep_IsRunning();

	// Despite the ActionList will be synced to the Client by the ActionComponent Class
	// We need to make sure the client knows the ActionComponent so the Get owning component could work on client.
	UPROPERTY(Replicated)
	UWaffActionComponent* OwningComponent;
	/*
	 * [Networking]
	 */

public:
	UFUNCTION()
	void Initialize(UWaffActionComponent* ActionComponent);
	
	/* Start immediately when added to the component */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;

	/* Action Nick Name to start/stop an action without referencing */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActonName;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	// Getters
	UFUNCTION(BlueprintCallable)
	UWaffActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;

	//[Networking] Make sure the class support networking, as UObject base class does not.
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
