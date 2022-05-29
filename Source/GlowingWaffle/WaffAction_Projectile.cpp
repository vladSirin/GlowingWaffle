// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAction_Projectile.h"

#include "WaffProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UWaffAction_Projectile::UWaffAction_Projectile()
{
	HandSocketName = "Muzzle_01";
	AttackDelay = 0.2f;
}

void UWaffAction_Projectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Get the character
	ACharacter* Character = Cast<ACharacter>(Instigator);

	// Using animation montage
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnimMontage, 1.0f, NAME_None);
	}

	/*
	 * [Networking] Binding and timer should only be set on server
	 * otherwise client will run and has null pointer issues for data not on clients
	 */
	if (Instigator->HasAuthority())
	{
		// Bind the timer handler delegates with specific var.
		AttackTimerDelegate.BindUFunction(this, FName("Attack_TimeElapsed"), Projectile, Instigator);
		// Call the Bound function after 0.2f sec
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, AttackTimerDelegate, AttackDelay, false);
	}
}

void UWaffAction_Projectile::Attack_TimeElapsed(TSubclassOf<AWaffProjectile> AttackProjectile, AActor* Instigator)
{
	// Get the character
	ACharacter* Character = Cast<ACharacter>(Instigator);

	// use the hand socket location as the spawn location
	FVector HandLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);

	// Do line trace to find the camera rotation hit target
	FHitResult OutHit;
	FVector Start;
	FRotator PlayerView;
	Character->GetController()->GetPlayerViewPoint(Start, PlayerView);

	FVector End = Start + PlayerView.Vector() * 1000;
	FCollisionQueryParams QueryParam;
	QueryParam.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParam);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);

	// Decide the Final rotation for the projectile to hit target
	FRotator ProjectileSpawnRotation;
	if (OutHit.GetActor())
	{
		ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, OutHit.ImpactPoint);
	}
	else
	{
		ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	}

	// Combine with the ControlRotation, so he actor will spawn at the hand position and facing the camera direction.
	FTransform Spawn_TM = FTransform(ProjectileSpawnRotation, HandLocation);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.Instigator = Character;

	//Play Muzzle Flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Character->GetMesh(), HandSocketName, FVector(ForceInit),
	                                       FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset, true,
	                                       EPSCPoolMethod::None, true);
	// Spawn the projectile actor at the hand of the character
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(AttackProjectile, Spawn_TM, SpawnParam);
	Character->GetCapsuleComponent()->IgnoreActorWhenMoving(NewProjectile, true);

	// Stop the action
	StopAction(Instigator);
}
