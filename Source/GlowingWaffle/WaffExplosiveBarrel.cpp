// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffExplosiveBarrel.h"
#include "WaffMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWaffExplosiveBarrel::AWaffExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SMComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleComp->SetupAttachment(SMComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(SMComp);
	SphereComp->SetSphereRadius(300.0f, true);

	ExplodeStrength = 5000.0f;
}

// Called when the game starts or when spawned
void AWaffExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorHit.AddDynamic(this, &AWaffExplosiveBarrel::OnHit);

	
}

// Called every frame
void AWaffExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaffExplosiveBarrel::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Cast<AWaffMagicProjectile>(OtherActor))
	{
		Explode();
	}
}

void AWaffExplosiveBarrel::Explode()
{
	TArray<UPrimitiveComponent*> CompInRange;
	SphereComp->GetOverlappingComponents(CompInRange);

	for (UPrimitiveComponent* InRange : CompInRange)
	{
		InRange->AddRadialForce(this->GetActorLocation(), SphereComp->GetScaledSphereRadius(), ExplodeStrength*InRange->GetMass(), ERadialImpulseFalloff::RIF_Constant, false);
	}
}
