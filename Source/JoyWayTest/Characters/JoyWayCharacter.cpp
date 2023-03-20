// Fill out your copyright notice in the Description page of Project Settings.


#include "JoyWayCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "../GameModes/JoyWayGameMode.h"
#include "../Weapon/Weapon.h"

// Sets default values
AJoyWayCharacter::AJoyWayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule component
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CapsuleComp);
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComponent->bUsePawnControlRotation = true;
	
	// Skeletal mesh component
	MeshComponent = GetMesh();
	check(MeshComponent);

	// Create health component that will be used when character gets damage
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);

	CurrentWeapon = nullptr;
	InventorySize = 2;
}

// Called when the game starts or when spawned
void AJoyWayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJoyWayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJoyWayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AJoyWayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJoyWayCharacter::MoveRight);

	// Bind rotation events
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind weapon events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJoyWayCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AJoyWayCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AJoyWayCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &AJoyWayCharacter::SelectWeapon_1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &AJoyWayCharacter::SelectWeapon_2);
}

void AJoyWayCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AJoyWayCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AJoyWayCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AJoyWayCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AJoyWayCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AJoyWayCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
	
	if (UWorld* World = GetWorld())
	{
		if (AJoyWayGameMode* GameMode = Cast<AJoyWayGameMode>(World->GetAuthGameMode()))
		{
			GameMode->PlayerDied(GetController());
			DestroyInventory();
		}
	}
}

void AJoyWayCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	auto CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	auto MovementComp = GetCharacterMovement();
	check(MovementComp);
	MovementComp->StopMovementImmediately();
	MovementComp->DisableMovement();
}

void AJoyWayCharacter::AttachWeapon(AWeapon* Weapon)
{
	if (!Weapon) return;
	if (Weapon == CurrentWeapon) return;
	
	if (IsWeaponFromInventory(Weapon))
	{
		Weapon->SetActorHiddenInGame(false);
	}
	else
	{
		if (!CanPutWeaponInInventory()) return;
		WeaponInventory.Add(Weapon);
	}

	auto CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	Weapon->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName());
	Weapon->SetActorRelativeLocation(WeaponAttachmentLocation);

	OnWeaponChanged.Broadcast(CurrentWeapon, Weapon);
	CurrentWeapon = Weapon;
}

void AJoyWayCharacter::SelectWeapon_1()
{
	if (!WeaponInventory.Num()) return;
	AttachWeapon(WeaponInventory[0]);
}

void AJoyWayCharacter::SelectWeapon_2()
{
	if (WeaponInventory.Num() != InventorySize) return;
	AttachWeapon(WeaponInventory[1]);
}

bool AJoyWayCharacter::IsWeaponFromInventory(AWeapon* Weapon)
{
	for (auto& Slot : WeaponInventory)
	{
		if (Weapon == Slot)
		{
			return true;
		}
	}
	return false;
}

bool AJoyWayCharacter::CanPutWeaponInInventory()
{
	return (WeaponInventory.Num() < InventorySize);
}

void AJoyWayCharacter::DestroyInventory()
{
	for (auto& Slot : WeaponInventory)
	{
		Slot->Destroy();
	}
}
