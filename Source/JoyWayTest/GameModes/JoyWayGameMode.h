// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JoyWayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AJoyWayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJoyWayGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called from died player
	UFUNCTION()
	void PlayerDied(AController* Player);

	virtual void BeginPlay() override;

private:
	// Respawn player after death
	UFUNCTION()
	void RespawnPlayer(AController* Player);

	float RespawnTime;

	// Handle to manage the timer
	FTimerHandle RespawnTimerHandle;
};
