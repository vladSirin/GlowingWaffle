// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffProjectile.h"

#include "WaffAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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



	// Setup initial values
	MoveComp->InitialSpeed = 1000.0f;
	MoveComp->SetVelocityInLocalSpace(GetActorForwardVector());
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.0f;
}

void AWaffProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
	// Bind function to Overlap and Hit for Sphere component.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWaffProjectile::OnOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &AWaffProjectile::OnHit);
}

// Called when the game starts or when spawned
void AWaffProjectile::BeginPlay()
{
	Super::BeginPlay();
}


// On overlap
void AWaffProjectile::OnOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	Explode();
	Destroy();
}

// On hit
void AWaffProjectile::OnHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
	Destroy();
}

// Explode
void AWaffProjectile::Explode_Implementation()
{
	// Ensure if it is still valid, deactivate comps, collisions and destroy it.
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
		ParticleComp->Deactivate();
		MoveComp->StopMovementImmediately();
		SetActorEnableCollision(false);
	}
}

// Called every frame
void AWaffProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


