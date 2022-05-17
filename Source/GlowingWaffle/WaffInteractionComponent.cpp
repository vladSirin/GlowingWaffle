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
	TraceDistance = 1000.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called every frame
void UWaffInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FindBestInteractable();
	// ...
}


void UWaffInteractionComponent::FindBestInteractable()
{
	// CVar for debug Draw
 	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
 
 	// Setup on the arguments for the solution
 	TArray<FHitResult> HitResults;
 	FVector EyeLocation;
 	FRotator EyeRotation;
 	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
 
 	FVector Start = EyeLocation;
 	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
 	FCollisionObjectQueryParams ObjectParam;
 	ObjectParam.AddObjectTypesToQuery(ECC_WorldDynamic);
 
 	// Now is the solution for check which object we want to interact with, multiple solutions
 
 	// Single Line Trace:
 	// FHitResult OutHit;
 	//GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParam);
 
 	// Multi Sweep Trace approach, FQuat is using Identity as default.
 	bool bIsBlocking = GetWorld()->SweepMultiByObjectType(HitResults, Start, End, FQuat::Identity, ObjectParam,
 	                                                      FCollisionShape::MakeSphere(TraceRadius));
 	FColor DebugColor = bIsBlocking ? FColor::Red : FColor::Green;

	// Clear the focused actor before trying to find one
	FocusedActor = nullptr;
	
 	// Check the hit actors and find one as focused actor
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
 				FocusedActor = HitResult.GetActor();
 				break;
 			}
 		}
 	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UWaffWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
 	if (bDebugDraw)
 	{
 		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 2.0f, 0, 2.9f);
 	}
}

void UWaffInteractionComponent::PrimaryInteract()
{
	if(ensure(FocusedActor))
	{
		IWaffGameplayInterface::Execute_Interact(FocusedActor, Cast<APawn>(GetOwner()));
	}
}
