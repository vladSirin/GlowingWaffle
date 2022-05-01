// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPickUp.h"

#include "WaffAttributeComponent.h"

// Sets default values
AWaffPickUp::AWaffPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Comps
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = SMComp;

	Cooldown = 10.0f;
}

// Called when the game starts or when spawned
void AWaffPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaffPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaffPickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	Disable();
	GetWorldTimerManager().ClearTimer(CooldownHandle);
	GetWorldTimerManager().SetTimer(CooldownHandle, this, &AWaffPickUp::Enable, Cooldown, false
		);

}

void AWaffPickUp::Disable()
{
	SMComp->SetVisibility(false);
	SetActorEnableCollision(false);
}


void AWaffPickUp::Enable()
{
	SMComp->SetVisibility(true);
	SetActorEnableCollision(true);
}

