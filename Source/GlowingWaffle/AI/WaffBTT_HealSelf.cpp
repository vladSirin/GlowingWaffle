// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffBTT_HealSelf.h"
#include "AIController.h"
#include "GlowingWaffle/WaffAttributeComponent.h"

void UWaffBTT_HealSelf::HealPerTick()
{
	if(AttributeComponent->IsAlive())
	{
		if(AttributeComponent->IsFullHealth())
		{
			FinishTask();
			return;
		}
		AttributeComponent->ApplyHealthChange(HealAmountPerTick);
	}
	else
	{
		AttributeComponent->GetOwner()->GetWorldTimerManager().ClearTimer(HealHandler);
	}
}

void UWaffBTT_HealSelf::FinishTask()
{
	AttributeComponent->GetOwner()->GetWorldTimerManager().ClearTimer(HealHandler);
	FAIMessage::Send(AIController, HealFinished);
}

void UWaffBTT_HealSelf::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID,
	bool bSuccess)
{
	Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
	StopWaitingForMessages(OwnerComp);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UWaffBTT_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	// This BTT is used for healing the owner until max health at X rate
	HealFinished.MessageName = "HealFinished";
	
	// Get Controller, then Pawn, to find the Attribute Component first
	AIController = OwnerComp.GetAIOwner();
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	AttributeComponent = Cast<UWaffAttributeComponent>(
		Pawn->GetComponentByClass(UWaffAttributeComponent::StaticClass()));

	// Base on Time, Apply health change until max health if not full health
	if(ensure(AttributeComponent) && !AttributeComponent->IsFullHealth())
	{
		Pawn->GetWorldTimerManager().ClearTimer(HealHandler);
		Pawn->GetWorldTimerManager().SetTimer<UWaffBTT_HealSelf>(HealHandler, this, &UWaffBTT_HealSelf::HealPerTick, HealRate, true);
		WaitForMessage(OwnerComp, "HealFinished");
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}
