// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::Yellow, float Duration = 5.0f)
{
	// Check if the UObject is valid
	if (!ensure(WorldContext))
	{
		return;
	}

	// Check if the World Context is valid
	UWorld* World = WorldContext->GetWorld();
	if (!ensure(WorldContext->GetWorld()))
	{
		return;
	}

	// Based on whether it is server and client, display different info to the screen
	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix+Msg, true);
	}
}
