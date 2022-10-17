// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/PickUpItems/UsableItems/JT_UsableItemBase.h"
#include "JT_FireWeaponBase.generated.h"

class AJT_Projectile;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReload);
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API AJT_FireWeaponBase : public AJT_UsableItemBase
{
	GENERATED_BODY()

		AJT_FireWeaponBase();
protected:


	bool IsReloading = false;


	UFUNCTION()
		void OnWeaponNeedReload();


public:

	virtual void UseBy_Implementation(AActor* InActor) override;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* GunMesh;
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UArrowComponent* ProjectileSpawnPoint;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<AJT_Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;


	FOnWeaponReload OnWeaponReloadBind;

	/**Damage on hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int Damage = 10;

	/**Current AmmoCount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int AmmoCount = 10;
	/**Max AmmoCount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int AmmoCountMax = 10;

	/**WeaponFunctions*/
	UFUNCTION(BlueprintCallable)
		void Fire();
	UFUNCTION(BlueprintCallable)
		void RefillAmmo();
	UFUNCTION(BlueprintCallable)
		bool GetIsReloding() const { return IsReloading; };
	/**~WeaponFunctions*/
};
