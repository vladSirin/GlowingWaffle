// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "WaffWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLOWINGWAFFLE_API UWaffWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// The Actor the UI is attached to
	UPROPERTY(BlueprintReadWrite, Category="UI")
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, Category="UI")
	FVector WorldOffset;
	
protected:

	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox;


	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
