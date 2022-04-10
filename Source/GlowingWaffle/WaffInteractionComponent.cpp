// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffInteractionComponent.h"
#include "WaffGameplayInterface.h"

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
	// Setup on the arguments for the solution
	APawn* MyPawn = Cast<APawn>(GetOwner());
	FHitResult OutHit;
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	FCollisionObjectQueryParams ObjectParame;
	ObjectParame.AddObjectTypesToQuery(ECC_WorldDynamic);

	// Now is the solution for check which object we want to interact with, multiple solutions

	// Single Line Trace:
	GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParame);

	// Check the hit actor
	AActor* HitActor = OutHit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<UWaffGameplayInterface>())
		{
			IWaffGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.9f);
}

// Called when the game starts
void UWaffInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWaffInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

