// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffBTS_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UWaffBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between Ai and the target and LOS
	UBlackboardComponent* MyBlackboard  = OwnerComp.GetBlackboardComponent();
	if(ensure(MyBlackboard))
	{
		if(const AActor* TargetActor = Cast<AActor>(MyBlackboard->GetValueAsObject("TargetActor")))
		{
			const AAIController* MyController = OwnerComp.GetAIOwner();
			const APawn* Pawn = MyController->GetPawn();
			if(ensure(Pawn))
			{
				const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(),
				                                           Pawn->GetActorLocation());
				
				const bool bWithinRange = DistanceTo < 2000.0f;
				bool bHasLOS = false;
				if(bWithinRange)
				{
					bHasLOS = MyController->LineOfSightTo(TargetActor);
				}
				MyBlackboard->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
			}
		}

	}

}
