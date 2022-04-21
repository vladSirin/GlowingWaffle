// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffDummyTarget.h"

// Sets default values
AWaffDummyTarget::AWaffDummyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = SMComp;

	AttriComp = CreateDefaultSubobject<UWaffAttributeComponent>(TEXT("AttributeComp"));
	AttriComp->OnHealthChanged.AddDynamic(this, &AWaffDummyTarget::OnHealthChanged);
}

// Called when the game starts or when spawned
void AWaffDummyTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaffDummyTarget::OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent* OwingComp, float NewHealth,
	float Delta)
{
	if(Delta < 0)
	{
		SMComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

// Called every frame
void AWaffDummyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

