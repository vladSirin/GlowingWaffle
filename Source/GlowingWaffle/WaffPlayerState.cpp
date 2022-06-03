// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPlayerState.h"

#include "Net/UnrealNetwork.h"

AWaffPlayerState::AWaffPlayerState()
{
	SetReplicates(true);
}

// Get player's num of Credit
float AWaffPlayerState::GetPlayerCredit() const
{
	return PlayerCredit.CreditNum;
}

bool AWaffPlayerState::ApplyCreditChange(float Delta)
{
	float OldCredit = PlayerCredit.CreditNum;
	PlayerCredit.CreditNum += Delta;
	FMath::Clamp(PlayerCredit.CreditNum, 0.0, PlayerCredit.CreditMax);
	PlayerCredit.LastDelta = PlayerCredit.CreditNum - OldCredit;

	// Delegate Broadcast credit changes
	OnCreditChanged.Broadcast(this, PlayerCredit.CreditNum, PlayerCredit.LastDelta);

	return PlayerCredit.LastDelta != 0;
}

// [Networking] Broadcast the event of credit change on Clients so that UI will update
void AWaffPlayerState::OnRep_Credit()
{
	OnCreditChanged.Broadcast(this, PlayerCredit.CreditNum, PlayerCredit.LastDelta);
}

void AWaffPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaffPlayerState, PlayerCredit);
}
