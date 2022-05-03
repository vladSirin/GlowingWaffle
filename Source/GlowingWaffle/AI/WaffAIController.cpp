// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAIController.h"
#include "Perception/AIPerceptionComponent.h"


AWaffAIController::AWaffAIController()
{
	TeamId = FGenericTeamId(1);
}

void AWaffAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetGenericTeamId(TeamId);
	RunBehaviorTree(BT);
}

FGenericTeamId AWaffAIController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type AWaffAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Super::GetTeamAttitudeTowards(Other);
}
