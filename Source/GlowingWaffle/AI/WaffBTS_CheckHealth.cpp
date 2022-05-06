// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffBTS_CheckHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GlowingWaffle/WaffAttributeComponent.h"

void UWaffBTS_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check if health is below threshold, if so, set bLowHealth to True
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(ensure(Blackboard))
	{
		APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
		if(ensure(Pawn))
		{
			UActorComponent* AttriComp = Pawn->GetComponentByClass(UWaffAttributeComponent::StaticClass());
			if(ensure(AttriComp))
			{
				if(Cast<UWaffAttributeComponent>(AttriComp)->GetHealthPercent() < LowHealthThreshold)
				{
					Blackboard->SetValueAsBool(bLowHeathKey.SelectedKeyName, true);
				}
			}
		}
	}
}
