// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWaffChest::AWaffChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChestBottomComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBottom"));
	ChestLidComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));

	RootComponent = ChestBottomComp;
	ChestLidComp->SetupAttachment(ChestBottomComp);

	// Open Replicates
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AWaffChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWaffChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaffChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened(); //Calling this still, cause RepNotify only works on clients
}

void AWaffChest::OnRep_LidOpened() const
{
	const float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	ChestLidComp->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

// Replicate the props to different clients
void AWaffChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaffChest, bLidOpened);
}
