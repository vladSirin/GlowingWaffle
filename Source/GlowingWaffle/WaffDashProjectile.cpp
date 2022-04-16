// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AWaffDashProjectile::AWaffDashProjectile()
{
	// bind the event on sphere comp so when hit in world , explode and teleport
	SphereComp->OnComponentHit.AddDynamic(this, &AWaffDashProjectile::OnHit);
	TimeBeforeExplode = 1.0f;
}

void AWaffDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
	// Set timer for explodes
	GetWorldTimerManager().SetTimer(TimHandler, this
	,&AWaffDashProjectile::Explode, TimeBeforeExplode, false);
}

void AWaffDashProjectile::Explode()
{
	// Not really, just stop movement and play the Particle effect. Save the current position
	MoveComp->StopMovementImmediately();
	TeleportLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEmitterTemp, GetActorLocation());

	// wait a while, then teleport player
	GetWorldTimerManager().ClearTimer(TimHandler);
	GetWorldTimerManager().SetTimer(TimHandler, this
	,&AWaffDashProjectile::Teleport, 0.2f, false);
}

void AWaffDashProjectile::Teleport()
{
	if(ensure(GetInstigator()))
	{
		GetInstigator()->TeleportTo(TeleportLocation, GetInstigator()->GetActorForwardVector().Rotation(), false, false);
		Destroy();
	}
}

void AWaffDashProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}
