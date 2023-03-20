// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OldHealth = Health = DefaultHealth = 1000.0f;
	MinHealth = 0.0f;

	DeathState = EDeathState::NotDead;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	check(Owner)
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

	OldHealth = Health = DefaultHealth;

}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetHealthPercent() const
{
	return Health / DefaultHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, MinHealth, DefaultHealth);

	if (OldHealth != Health)
	{
		OldHealth = Health;

		OnHealthChanged.Broadcast(this, OldHealth, Health, GetHealthPercent());

		if (!Health && DeathState == EDeathState::NotDead)
		{
			StartDeath();
			FinishDeath();
		}
	}
}

void UHealthComponent::StartDeath()
{
	if (DeathState != EDeathState::NotDead)
	{
		return;
	}

	DeathState = EDeathState::DeathStarted;

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);
}

void UHealthComponent::FinishDeath()
{
	if (DeathState != EDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EDeathState::DeathFinished;

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);
}
