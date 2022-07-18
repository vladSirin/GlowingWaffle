// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffActionEffect.h"

#include "GameFramework/GameStateBase.h"

UWaffActionEffect::UWaffActionEffect()
{
	bAutoStart = true;
}

void UWaffActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if(Duration > 0.0f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, DurationDelegate, Duration, false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, PeriodDelegate, Period, false);
	}
}

void UWaffActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	UWaffActionComponent* ActionComponent = GetOwningComponent();
	if(ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}

float UWaffActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if(GS)
	{
		float TimeEnd = Duration + TimeStarted;
		return TimeEnd - GS->GetServerWorldTimeSeconds();
	}
	return Duration;
}

void UWaffActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
	
}
