// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffMagicProjectile.h"

#include "WaffActionComponent.h"
#include "WaffGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWaffMagicProjectile::AWaffMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
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
	if (OtherActor && OtherActor != GetInstigator())
	{
		UWaffActionComponent* ActionComponent = Cast<UWaffActionComponent>(OtherActor->GetComponentByClass(UWaffActionComponent::StaticClass()));
		if(ActionComponent && ActionComponent->ActiveGameplayTag.HasTag(ParryTag))
		{
			MoveComp->Velocity = - MoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if(UWaffGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();

			// Applying burning effect
			if(ActionComponent && HasAuthority())
			{
				ActionComponent->AddAction(GetInstigator(), EffectActionClass);
			}
		}
	}
}

// Called every frame
void AWaffMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
