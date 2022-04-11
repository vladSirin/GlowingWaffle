// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffChest.h"

// Sets default values
AWaffChest::AWaffChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChestBottmComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBottom"));
	ChestLidComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));

	ChestBottmComp->SetupAttachment(RootComponent);
	ChestLidComp->SetupAttachment(ChestBottmComp);

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
	ChestLidComp->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}
