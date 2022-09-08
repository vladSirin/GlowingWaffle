// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAnimInstance.h"

#include "GameplayTagContainer.h"
#include "WaffActionComponent.h"

void UWaffAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if(OwningActor)
	{
		ActionComp = Cast<UWaffActionComponent>(OwningActor->GetComponentByClass(UWaffActionComponent::StaticClass()));
	}
}

void UWaffAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if(ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTag.HasTag(StunnedTag);
	}
}
