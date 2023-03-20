// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, const FHitResult&, HitResult);

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class JOYWAYTEST_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FOnHit OnHitDelegate;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Called when projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
