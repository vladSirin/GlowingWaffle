// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPickUp.h"

#include "Net/UnrealNetwork.h"

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
	bActive = true;

	bReplicates = true;
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
	//To be implement...
}

void AWaffPickUp::SetActiveState(bool bActiveState)
{
	// Only Server updates the active state
	if (HasAuthority())
	{
		bActive = bActiveState;
	}
	RootComponent->SetVisibility(bActive, true);
	SetActorEnableCollision(bActive);
}

// Change visibility and collision based on state on client when state is replicated
void AWaffPickUp::OnRep_IsActive()
{
	SetActiveState(bActive);
}

void AWaffPickUp::ShowUp()
{
	SetActiveState(true);
}


void AWaffPickUp::HideAndCoolDown()
{
	SetActiveState(false);
	GetWorldTimerManager().ClearTimer(CooldownHandle);
	GetWorldTimerManager().SetTimer(CooldownHandle, this, &AWaffPickUp::ShowUp, Cooldown, false
	);
}

void AWaffPickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaffPickUp, bActive);
}
