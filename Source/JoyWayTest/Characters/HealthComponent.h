// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealth_AttributeChanged, UHealthComponent*, HealthComponent, float, OldValue, float, NewValue, float, percent);

// Current state of death
UENUM(BlueprintType)
enum class EDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOYWAYTEST_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathFinished;

	// Get current health
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	// Get normalized current health
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Health")
	static UHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHealthComponent>() : nullptr); }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Health on spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth;

private:
	// Called as callback inside actor
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Begins death sequence for the owner
	void StartDeath();

	// Finishes death sequence for the owner
	void FinishDeath();

	// State to handle dying
	EDeathState DeathState;

	// Current and minimum health
	float Health;
	float MinHealth;
	float OldHealth;
};
