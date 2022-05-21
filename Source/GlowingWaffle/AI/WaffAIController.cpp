// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"


AWaffAIController::AWaffAIController()
{
	TeamId = FGenericTeamId(1);
}

void AWaffAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TargetActorKey = "TargetActor";
	SetGenericTeamId(TeamId);
	RunBehaviorTree(BT);
}

void AWaffAIController::SetTargetActor(AActor* InstigatorActor)
{	
	AActor* OldTarget = GetTargetActor();
	UBlackboardComponent* BB = GetBlackboardComponent();
	BB->SetValueAsObject(TargetActorKey, InstigatorActor);
	if(OldTarget != InstigatorActor)
	{
		OnTargetFirstSight.Broadcast(InstigatorActor);
	}
}

AActor* AWaffAIController::GetTargetActor()
{
	const UBlackboardComponent* BB = GetBlackboardComponent();
	return Cast<AActor>(BB->GetValueAsObject(TargetActorKey));
}

FGenericTeamId AWaffAIController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type AWaffAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Super::GetTeamAttitudeTowards(Other);
}
