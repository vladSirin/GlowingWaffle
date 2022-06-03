// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPlayerState.h"

#include "Net/UnrealNetwork.h"

AWaffPlayerState::AWaffPlayerState()
{
	Credit = 0.0f;
	CreditMax = 999.f;
	SetReplicates(true);
}

float AWaffPlayerState::GetPlayerCredit() const
{
	return Credit;
}

bool AWaffPlayerState::ApplyCreditChange(float Delta)
{
	float OldCredit = Credit;
	Credit += Delta;
	FMath::Clamp(Credit, 0.0, CreditMax);
	float ActualDelta = Credit - OldCredit;

	// Delegate Broadcast credit changes
	OnCreditChanged.Broadcast(this, Credit, ActualDelta);

	return ActualDelta != 0;
}

void AWaffPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaffPlayerState, Credit);
	DOREPLIFETIME(AWaffPlayerState, CreditMax);
}
