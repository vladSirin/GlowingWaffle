// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaffWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "WaffInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOWINGWAFFLE_API UWaffInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaffInteractionComponent();

	// The primary interaction function
	void PrimaryInteract();

protected:

	// Reliable, request will always arrive, eventually. Request will resent unless and acknowledgement is received.
	// Unreliable, not guaranteed, packet can be lost and will not retry.
	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void FindBestInteractable();

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category="Interact")
	TSubclassOf<UWaffWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UWaffWorldUserWidget* DefaultWidgetInstance;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
