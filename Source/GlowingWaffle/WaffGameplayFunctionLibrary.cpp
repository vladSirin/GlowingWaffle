// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffGameplayFunctionLibrary.h"

#include "WaffActionComponent.h"
#include "WaffAttributeComponent.h"

bool UWaffGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (UWaffAttributeComponent* AttributeComponent = UWaffAttributeComponent::GetAttributes(TargetActor))
	{
		return AttributeComponent->ApplyHealthChange(-DamageAmount, DamageCauser);
	}
	return false;
}

bool UWaffGameplayFunctionLibrary::ConsumeRage(AActor* Instigator,AActor* TargetActor, float ConsumeAmount)
{
	if(UWaffAttributeComponent* AttributeComponent = UWaffAttributeComponent::GetAttributes(TargetActor))
	{
		return AttributeComponent->ApplyRageChange(-ConsumeAmount, Instigator);
	}
	return false;
}

bool UWaffGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
                                                          const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
		Direction.Normalize();
		
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint,
			                              HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool UWaffGameplayFunctionLibrary::AddAction(AActor* Instigator, TSubclassOf<UWaffAction> ActionClass)
{
	if(UWaffActionComponent* ActionComponent = UWaffActionComponent::GetActionComponent(Instigator))
	{
		return ActionComponent->AddAction(Instigator, ActionClass);
	}
	return false;
}
