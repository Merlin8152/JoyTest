// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjects/PickUpItems/UsableItems/FireWeapon/JT_FireWeaponBase.h"

#include "Character/JT_CharacterBase.h"
#include "InteractiveObjects/PickUpItems/UsableItems/FireWeapon/JT_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

AJT_FireWeaponBase::AJT_FireWeaponBase()
{

	// Create a gun mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));

	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	GunMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GunMesh);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	ProjectileSpawnPoint->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void AJT_FireWeaponBase::UseBy_Implementation(AActor* InActor)
{
	Super::UseBy_Implementation(InActor);

	if (AJT_CharacterBase* Character = Cast<AJT_CharacterBase>(InActor))
	{
		Character->EquipWeapon(this);
	}
}

void AJT_FireWeaponBase::OnWeaponNeedReload()
{
	IsReloading = true;
	OnWeaponReloadBind.Broadcast();
}



void AJT_FireWeaponBase::Fire()
{
	if (IsReloading) return;

	if (AmmoCount == 0)
	{
		OnWeaponNeedReload();
		return;
	}

	--AmmoCount;

	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = (ProjectileSpawnPoint != nullptr) ? ProjectileSpawnPoint->GetComponentRotation() : GetActorRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((ProjectileSpawnPoint != nullptr) ? ProjectileSpawnPoint->GetComponentLocation() : GetActorLocation());

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile
			AJT_Projectile* Projectile = World->SpawnActor<AJT_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)	Projectile->SetDamage(Damage);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AJT_FireWeaponBase::RefillAmmo()
{
	AmmoCount = AmmoCountMax;

	IsReloading = false;
}
