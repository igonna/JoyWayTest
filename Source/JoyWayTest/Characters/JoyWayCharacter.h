// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JoyWayCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, AWeapon*, OldWeapon, AWeapon*, NewWeapon);

class UCameraComponent;
class UCapsuleComponent;
class UHealthComponent;
class USkeletalMeshComponent;
class AWeapon;

UCLASS()
class JOYWAYTEST_API AJoyWayCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJoyWayCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AttachWeapon(AWeapon* Weapon);

	// On weapon changed delegate
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	void SelectWeapon_1();
	void SelectWeapon_2();

	// Return true if enough space
	bool CanPutWeaponInInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handles moving forward/backward
	void MoveForward(float Val);

	// Handles stafing movement, left and right
	void MoveRight(float Val);

	// Weapon actions
	void StartFire();
	void StopFire();
	void ReloadWeapon();

	// Health on spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponAttachmentLocation;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	// Start death sequence
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	void DisableMovementAndCollision();

	AWeapon* CurrentWeapon;

	TArray<AWeapon*> WeaponInventory;

	uint16 InventorySize;

	bool IsWeaponFromInventory(AWeapon* Weapon);

	// Destroy inventory on death
	void DestroyInventory();
};
