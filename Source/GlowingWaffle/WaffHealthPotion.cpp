// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffHealthPotion.h"
#include "WaffAttributeComponent.h"
#include "Engine/ActorChannel.h"

// Sets default values
AWaffHealthPotion::AWaffHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaffHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWaffHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaffHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if (InstigatorPawn)
	{
		UWaffAttributeComponent* AttriComp =  Cast<UWaffAttributeComponent>(InstigatorPawn->GetComponentByClass(UWaffAttributeComponent::StaticClass()));
		if (AttriComp && !AttriComp->IsFullHealth())
		{
			if(AttriComp->ApplyHealthChange(HealValue, InstigatorPawn))
			{
				HideAndCoolDown();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("WaffHealthPotion, No Heal, Full Health or Null Pawn!"))
			return;
		}
	}
}
