// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffMagicProjectile.h"

#include "WaffAttributeComponent.h"
#include "WaffGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWaffMagicProjectile::AWaffMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaffMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AWaffMagicProjectile::OnOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                SweepResult);
	if (OtherActor && OtherActor != GetInstigator())
	{
		UWaffGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);
	}
}

// Called every frame
void AWaffMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
