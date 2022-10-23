// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "JT_CharacterBase.generated.h"


class AJT_PlayerController;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

class UJT_GameInstance;
class AJoyTestProjectile;

class UJT_InventoryComponent;
class AJT_FireWeaponBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChange, float, NewHP, float, MaxHP);

UCLASS()
class JOYTEST_API AJT_CharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle UpdateTargetViewTimerHandle;

	int32 TempQuickSlotIndex = -1;


	/*HP*/
	/*
		Current HP
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0", UIMin = "1.0"), Category = "PlayerCharacter")
		float HPCurrent = 100.0f;

	/*
		Max HP
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0", UIMin = "1.0"), Category = "PlayerCharacter")
		float HPMax = 100.0f;
	/*~HP*/


	/*Weapon*/
	UPROPERTY(BlueprintReadOnly, Category = "PlayerCharacter")
		AJT_FireWeaponBase* EquipedWeapon = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "PlayerCharacter")
		//int32 EquipedSlotIndex = -1;
	/*~Weapon*/


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter")
		AActor* TargetActor;


	// Components //

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UJT_InventoryComponent* InventoryComponent;

	// ~Components //



	UFUNCTION()
		void OnWeaponReload();
	UFUNCTION()
		void RefillWeaponAmmo();
	UFUNCTION()
		void OnQuickSlotChange(int32 Index);


public:
	// Sets default values for this character's properties
	AJT_CharacterBase();


	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);
	/** Handles stafing movement, left and right */
	void MoveRight(float Val);


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);



	/*Call on load map*/
	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "PlayerCharacter")
		void OnGameStart(); //only on server
	virtual void OnGameStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "BaseCharacter")
		void FirstSpawnUpdate(); //only on server
	virtual void FirstSpawnUpdate_Implementation();



	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void ChangeHP(float Delta);
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "PlayerCharacter")
		void EquipWeapon(AJT_FireWeaponBase* Weapon);
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "PlayerCharacter")
		void UnequipWeapon();
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "PlayerCharacter")
		void UseQuickSlot(int QuickSlotIndex);
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void Death();


	/*OnPressButtons Events*/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		void OnFire();
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Inventory")
		void OnInventoryPressed();
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Interact")
		void OnInteractPressed();
	/*~OnPressButtons Events*/

	/*FindTagetActor*/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void SetTargetByScreenCenter();
	/*FindTagetActor*/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|TargetItem")
		void UpdateTargetView();


	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|TargetItem")
		void ClearTarget();
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|TargetItem")
		void AddTarget(AActor* InteractiveActor);


	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	/** Returns InventoryComponent subobject **/
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		UJT_InventoryComponent* GetInventoryComponent()const { return InventoryComponent; };


	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		float GetHP() { return HPCurrent; };
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		float GetHPMax() { return HPMax; };


	/*Objects links*/
	UPROPERTY(BlueprintReadOnly, Category = "PlayerCharacter")
		AJT_PlayerController* MyPlayerController = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "BaseCharacter")
		UJT_GameInstance* MyGameInstance = nullptr;
	/*~Objects links*/



	/*
		Time between searching new interactive item.
		In seconds.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "PlayerCharacter|TargetItem")
		float TargetUpdateInterval = 0.2;

	/*
		ScreenTargetRadius
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1.0", UIMin = "1.0"), Category = "PlayerCharacter|TargetItem")
		float ScreenTargetRadius = 250.0f;



	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* ReloadAnimation;

	/** HP Change Bind */
	UPROPERTY(BlueprintAssignable, Category = Gameplay)
		FOnHPChange OnHPChangeBind;
};
