// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "WaffActionComponent.generated.h"

class UWaffAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLOWINGWAFFLE_API UWaffActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Tags")
	FGameplayTagContainer ActiveGameplayTag;
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	bool AddAction(AActor* Instigator, TSubclassOf<UWaffAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(UWaffAction* Action);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	// The list of Class of default actions
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	TArray<TSubclassOf<UWaffAction>> DefaultActions;
	
	// Sets default values for this component's properties
	UWaffActionComponent();

	// Static Function, get Action Component
	UFUNCTION(BlueprintCallable, Category="Actions")
	static UWaffActionComponent* GetActionComponent(AActor* FromActor);

	// Static FUnction, check if action already exist
	UFUNCTION(BlueprintCallable, Category="Actions")
	static  bool IsActionExist(AActor* FromActor, TSubclassOf<UWaffAction> ActionClass);

protected:
	UPROPERTY()
	TArray<UWaffAction*> ActionList;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_StartAction(AActor* Instigator, FName ActionName);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
