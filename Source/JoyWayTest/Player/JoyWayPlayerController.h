// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JoyWayPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPossessedPawnChanged, APawn*, OldPawn, APawn*, NewPawn);

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AJoyWayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Called when the possessed pawn changes (either OldPawn or NewPawn might be nullptr)
	UPROPERTY(BlueprintAssignable, Category = "Pawn")
	FOnPossessedPawnChanged OnPossessedPawnChanged;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
