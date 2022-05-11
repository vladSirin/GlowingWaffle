// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffInteractionComponent.h"
#include "WaffGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Console variables
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.DebugDrawInteraction"), false, TEXT("Draw Debug info for Interaction Components"),
	ECVF_Cheat);

// Sets default values for this component's properties
UWaffInteractionComponent::UWaffInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWaffInteractionComponent::PrimaryInteract()
{
	// CVar for debug Draw
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	// Setup on the arguments for the solution
	APawn* MyPawn = Cast<APawn>(GetOwner());
	TArray<FHitResult> HitResults;
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	FCollisionObjectQueryParams ObjectParam;
	ObjectParam.AddObjectTypesToQuery(ECC_WorldDynamic);

	// Now is the solution for check which object we want to interact with, multiple solutions

	// Single Line Trace:
	// FHitResult OutHit;
	//GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParam);

	// Multi Sweep Trace approach, FQuat is using Identity as default.
	bool bIsBlocking = GetWorld()->SweepMultiByObjectType(HitResults, Start, End, FQuat::Identity, ObjectParam,
	                                                      FCollisionShape::MakeSphere(30.0f));
	FColor DebugColor = bIsBlocking ? FColor::Red : FColor::Green;

	// Check the hit actor
	for (FHitResult HitResult : HitResults)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 30.0f, 32, DebugColor, false, 2.0f, 0, 2.0f);
		}
		if (HitResult.GetActor())
		{
			if (HitResult.GetActor()->Implements<UWaffGameplayInterface>())
			{
				IWaffGameplayInterface::Execute_Interact(HitResult.GetActor(), MyPawn);
				break;
			}
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 2.0f, 0, 2.9f);
	}
}

// Called when the game starts
void UWaffInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UWaffInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
