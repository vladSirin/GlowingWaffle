// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AWaffAIController::AWaffAIController()
{
	
}

void AWaffAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BT);

	if(APawn* MyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn); 
	}
}
