// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffProjectile.h"

#include "WaffAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWaffProjectile::AWaffProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the necessary components and attachments.
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(SphereComp);

	// Setup collision for sphere comp
	SphereComp->SetCollisionProfileName("Projectile", true);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWaffProjectile::OnOverlap);

	// Setup initial values
	MoveComp->InitialSpeed = 1000.0f;
	MoveComp->SetVelocityInLocalSpace(GetActorForwardVector());
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AWaffProjectile::BeginPlay()
{
	Super::BeginPlay();
}


// On overlap, if OtherActor has AttributeComponent, deal damage and destroy
void AWaffProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this->GetInstigator())   
	{
		UActorComponent* AttriComp = OtherActor->GetComponentByClass(UWaffAttributeComponent::StaticClass());
		if(AttriComp)
		{
			Cast<UWaffAttributeComponent>(AttriComp)->OnHealthChange(-DamageOnHit);
			Destroy();
		};

	}
}

// Called every frame
void AWaffProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
