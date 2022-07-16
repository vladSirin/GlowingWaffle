// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Media/Public/IMediaControls.h"

AWaffPlayerController::AWaffPlayerController()
{
	TeamId = FGenericTeamId(0);
}

void AWaffPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AWaffPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AWaffPlayerController::TogglePauseMenu);
}

void AWaffPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChange.Broadcast(InPawn);
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

void AWaffPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateChanged.Broadcast(PlayerState);
}

void AWaffPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	BlueprintBeginPlayingState();
}
