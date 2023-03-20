// Fill out your copyright notice in the Description page of Project Settings.

#include "JoyWayGameMode.h"

AJoyWayGameMode::AJoyWayGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RespawnTime = 3.0f;
}

void AJoyWayGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AJoyWayGameMode::PlayerDied(AController* Player)
{
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindUFunction(this, FName("RespawnPlayer"), Player);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, RespawnTime, false);
	}
}

void AJoyWayGameMode::RespawnPlayer(AController* Player)
{
	if (!Player) return;

	AActor* PlayerStart = FindPlayerStart(Player);

	if (!PlayerStart) return;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

	if (!NewPawn) return;

	APawn* CurrentPawn = Player->GetPawn();

	Player->Possess(NewPawn);

	if (CurrentPawn)
	{
		CurrentPawn->Destroy();
	}
}
