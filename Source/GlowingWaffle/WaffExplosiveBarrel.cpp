// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffExplosiveBarrel.h"

#include "WaffAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AWaffExplosiveBarrel::AWaffExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = SMComp;

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
	if (OtherActor)
	{
		Explode();
		UE_LOG(LogTemp, Log, TEXT("Hit by Projectile, Explode"));

		// %s = string
		// %f = floatx
		// logs: "OtherActor: MyActor_1, at gametime: 124.4"
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor),
		       GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit At Location: %s"), *Hit.ImpactNormal.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactNormal, CombinedString, nullptr, FColor::Green, 2.0f, true);

		// Detailed info on logging in ue4
		// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
	}
}

void AWaffExplosiveBarrel::Explode()
{
	ForceComp->FireImpulse();
}
