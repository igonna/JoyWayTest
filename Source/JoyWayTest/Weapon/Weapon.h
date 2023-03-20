// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentMagazineSizeChanged, int32, Size);

// Weapon fire mode
UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single = 0,
	Auto
};

class USphereComponent;
class AJoyWayCharacter;
class UPickUpComponent;

UCLASS()
class JOYWAYTEST_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

	// Fire a Projectile
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StartFire();

	// Fire a Projectile
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Reload")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Magazine")
	int32 GetCurrentMagazineSize() const;

	UFUNCTION(BlueprintCallable, Category = "Magazine")
	int32 GetDefaultMagazineSize() const;

	// Delegate will be called on current magazine size change
	UPROPERTY(BlueprintAssignable)
	FOnCurrentMagazineSizeChanged OnCurrentMagazineSizeChanged;

protected:
	virtual void BeginPlay() override;

	// Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	// Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MaxDamage;

	// Ammo count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 DefaultMagazineSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	EFireMode FireMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float FireRate;
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* ProjectileLocationSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (AllowPrivateAccess = "true"))
	UPickUpComponent* PickUpComponent;

	UFUNCTION()
	void ApplyDamage(const FHitResult& HitResult);

	// Fire a Projectile
	void Fire();

	// true if active firing
	bool bFireActive;

	float LastFireTime;

	int32 CurrentMagazineSize;

	UFUNCTION()
	void OnPickUp(AJoyWayCharacter* Character);
};
