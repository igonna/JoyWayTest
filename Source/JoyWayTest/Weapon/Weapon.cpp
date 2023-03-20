// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "PickUpComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	PickUpComponent = CreateDefaultSubobject<UPickUpComponent>("PickUpComponent");
	RootComponent = PickUpComponent;
	PickUpComponent->OnPickUp.AddDynamic(this, &AWeapon::OnPickUp);

	ProjectileLocationSphere = CreateDefaultSubobject<USphereComponent>("ProjectileLocationSphere");
	ProjectileLocationSphere->SetupAttachment(RootComponent);
	ProjectileLocationSphere->SetCollisionProfileName("NoCollision");
	ProjectileLocationSphere->SetComponentTickEnabled(false);
	ProjectileLocationSphere->InitSphereRadius(1.0f);

	MinDamage = 90.0f;
	MaxDamage = 100.0f;
	DefaultMagazineSize = 10;

	FireMode = EFireMode::Single;
	bFireActive = false;
	FireRate = 0.1f;
	LastFireTime = 0.0f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Reload();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFireActive)
	{
		Fire();
	}

}

void AWeapon::StartFire()
{
	if (FireMode == EFireMode::Auto)
	{
		bFireActive = true;
	}
	else if (FireMode == EFireMode::Single)
	{
		Fire();
	}
}

void AWeapon::StopFire()
{
	if (FireMode == EFireMode::Auto)
	{
		bFireActive = false;
	}
}

void AWeapon::Fire()
{
	// Try and fire a projectile
	if (!ProjectileClass) return;

	if (UWorld* const World = GetWorld())
	{
		float CurrentTime = World->GetTimeSeconds();

		if ((CurrentTime - LastFireTime) > FireRate)
		{
			LastFireTime = CurrentTime;
		}
		else
		{
			return;
		}

		if (!CurrentMagazineSize) return;
		else
		{
			CurrentMagazineSize -= 1;
			OnCurrentMagazineSizeChanged.Broadcast(CurrentMagazineSize);
		}

		
			
		const FVector SpawnLocation = ProjectileLocationSphere->GetComponentLocation();
		const FRotator SpawnRotation = ProjectileLocationSphere->GetComponentRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			
		if (Projectile)
		{
			Projectile->OnHitDelegate.AddDynamic(this, &AWeapon::ApplyDamage);
		}
	}
}

void AWeapon::ApplyDamage(const FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();
	
	if (!DamagedActor) return;

	if (UWorld* const World = GetWorld())
	{
		if (APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0))
		{
			TSubclassOf<UDamageType> DamageType;

			UGameplayStatics::ApplyDamage(DamagedActor, FMath::RandRange(MinDamage, MaxDamage), Controller, this, DamageType);
		}
	}
}

void AWeapon::Reload()
{
	CurrentMagazineSize = DefaultMagazineSize;
	OnCurrentMagazineSizeChanged.Broadcast(CurrentMagazineSize);
}

void AWeapon::OnPickUp(AJoyWayCharacter* Character)
{
	if (!Character) return;

	Character->AttachWeapon(this);
}

int32 AWeapon::GetCurrentMagazineSize() const
{
	return CurrentMagazineSize;
}

int32 AWeapon::GetDefaultMagazineSize() const
{
	return DefaultMagazineSize;
}