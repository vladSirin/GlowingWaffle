// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffBTT_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GlowingWaffle/WaffAttributeComponent.h"

UWaffBTT_RangedAttack::UWaffBTT_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type UWaffBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Spawn Projectile to Hit the target,
	// Calculate the Spawn Transform (Location and Rotation), and Set Spawn params

	// Get Controller
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if(ensure(Controller))
	{
		// Initialize Params, SpawnLocation
		FActorSpawnParameters SpawnParameters;
		FVector SpawnLocation;

		// Make sure there is a Pawn owning the BT
		APawn* Pawn = Controller->GetPawn();
		if(ensure(Pawn))
		{
			// Defne the spawn location and Params
			SpawnLocation = Cast<ACharacter>(Pawn)->GetMesh()->GetSocketLocation("Muzzle_01");
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.Instigator = Pawn;
		}

		// Get and cast the targetActor
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		// Sometimes there is no target, then return failed
		if(TargetActor == nullptr || !UWaffAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		// Calculate the Spawn Rotation
		const FVector Direction = TargetActor->GetActorLocation() - SpawnLocation;
		FRotator SpawnRotation = Direction.Rotation();
		SpawnRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		SpawnRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		// All Set, spawn the projectile
		const AActor* SpawnedProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation,SpawnRotation, SpawnParameters);

		// If spawn failed
		return SpawnedProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	// If no controller.
	return  EBTNodeResult::Failed;
}
