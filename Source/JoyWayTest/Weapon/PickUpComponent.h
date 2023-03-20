// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "../Characters/JoyWayCharacter.h"
#include "PickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AJoyWayCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYWAYTEST_API UPickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPickUpComponent();

	// Delegate to receive pick up event
	UPROPERTY(BlueprintAssignable, Category = "PickUp")
	FOnPickUp OnPickUp;

private:
	virtual void BeginPlay() override;

	// Execute when something overlaps this component
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
