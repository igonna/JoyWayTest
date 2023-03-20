// Fill out your copyright notice in the Description page of Project Settings.


#include "JoyWayPlayerController.h"
#include "GameFramework/PlayerController.h"

void AJoyWayPlayerController::OnPossess(APawn* InPawn)
{
	APawn* CurrentPawn = GetPawn();

	if (CurrentPawn != InPawn)
	{
		OnPossessedPawnChanged.Broadcast(CurrentPawn, InPawn);
	}

	Super::OnPossess(InPawn);
}

void AJoyWayPlayerController::OnUnPossess()
{
	APawn* CurrentPawn = GetPawn();

	OnPossessedPawnChanged.Broadcast(CurrentPawn, nullptr);

	Super::OnUnPossess();
}
