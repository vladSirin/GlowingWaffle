// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffExplosiveBarrel.h"
#include "WaffMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AWaffExplosiveBarrel::AWaffExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = SMComp;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleComp->SetupAttachment(SMComp);
	ParticleComp->SetAutoActivate(false);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	ForceComp->SetupAttachment(SMComp);

	ForceComp->Radius = 700.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->SetAutoActivate(false);
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AWaffExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	this->OnActorHit.AddDynamic(this, &AWaffExplosiveBarrel::OnHit);
}

void AWaffExplosiveBarrel::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AWaffMagicProjectile>(OtherActor))
	{
		Explode();
		ParticleComp->Activate();
	}
}

void AWaffExplosiveBarrel::Explode()
{
	ForceComp->FireImpulse();
}
