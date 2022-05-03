// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPlayerController.h"

AWaffPlayerController::AWaffPlayerController()
{
	TeamId = FGenericTeamId(0);
}

FGenericTeamId AWaffPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AWaffPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetGenericTeamId(TeamId);
}
