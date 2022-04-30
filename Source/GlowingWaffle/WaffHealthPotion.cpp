// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffHealthPotion.h"
#include "WaffAttributeComponent.h"
#include "Engine/ActorChannel.h"

// Sets default values
AWaffHealthPotion::AWaffHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Comps
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = SMComp;

	Cooldown = 10.0f;
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
	if (InstigatorPawn)
	{
		UWaffAttributeComponent* AttriComp =  Cast<UWaffAttributeComponent>(InstigatorPawn->GetComponentByClass(UWaffAttributeComponent::StaticClass()));
		if (AttriComp && !AttriComp->IsFullHealth())
		{
			AttriComp->ApplyHealthChange(50);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("WaffHealthPotion, No Heal, Full Health or Null Pawn!"))
			return;
		}
	}
	Disable();
	GetWorldTimerManager().ClearTimer(CooldownHandle);
	GetWorldTimerManager().SetTimer(CooldownHandle, this, &AWaffHealthPotion::Enable, 10.0f, false
		);
}

void AWaffHealthPotion::Disable()
{
	SMComp->SetVisibility(false);
	SetActorEnableCollision(false);
}

void AWaffHealthPotion::Enable()
{
	SMComp->SetVisibility(true);
	SetActorEnableCollision(true);
}
