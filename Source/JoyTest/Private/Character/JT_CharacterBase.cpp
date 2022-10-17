// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JT_CharacterBase.h"

#include "GameObjects/JT_PlayerController.h"
#include "GameObjects/JT_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

#include "InteractiveObjects/JT_InteractiveObjectInterface.h"
#include "GameObjects/JT_GameHUD.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "InteractiveObjects/PickUpItems/UsableItems/JT_UsableItemBase.h"


#include "Components/Inventory/JT_InventoryComponent.h"

#include "InteractiveObjects/PickUpItems/UsableItems/FireWeapon/JT_FireWeaponBase.h"

// Sets default values
AJT_CharacterBase::AJT_CharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	InventoryComponent = CreateDefaultSubobject<UJT_InventoryComponent>(TEXT("InventoryComponent"));

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

}



void AJT_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	MyPlayerController = Cast<AJT_PlayerController>(NewController);

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("UpdateTargetView"));
	GetWorldTimerManager().SetTimer(UpdateTargetViewTimerHandle, TimerDel, TargetUpdateInterval, true);
}

void AJT_CharacterBase::UnPossessed()
{
	Super::UnPossessed();

	MyPlayerController = nullptr;

	GetWorldTimerManager().ClearTimer(UpdateTargetViewTimerHandle);
}

void AJT_CharacterBase::OnWeaponReload()
{
	if (!IsValid(EquipedWeapon)) return;

	EquipedWeapon->Fire();

	if (ReloadAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);

			AnimInstance->Montage_Play(ReloadAnimation, 1.f, EMontagePlayReturnType::MontageLength, 0.0f, true);


			FOnMontageEnded LEndDelegate;
			LEndDelegate.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) { this->RefillWeaponAmmo(); });
			AnimInstance->Montage_SetEndDelegate(LEndDelegate, ReloadAnimation);
		}
	}
	else
	{
		RefillWeaponAmmo();
	}
}

void AJT_CharacterBase::RefillWeaponAmmo()
{
	if (!IsValid(EquipedWeapon)) return;

	EquipedWeapon->RefillAmmo();
}

void AJT_CharacterBase::OnQuickSlotChange(int32 Index)
{
	if (InventoryComponent->GetIsQuickSlotEmpty(Index)) return;

	if (EquipedSlotIndex == Index) UnequipWeapon();
}

void AJT_CharacterBase::EquipWeapon(AJT_FireWeaponBase* Weapon)
{
	bool IsNeedEquip = true;
	if (IsValid(EquipedWeapon))
	{
		if (EquipedWeapon->IsA(Weapon->ItemInfo->ItemClass)) IsNeedEquip = false;

		UnequipWeapon();
	}

	if (!IsNeedEquip) return;

	auto CreatedWeapon = GetWorld()->SpawnActor(Weapon->ItemInfo->ItemClass);
	CreatedWeapon->DisableComponentsSimulatePhysics();
	CreatedWeapon->SetActorEnableCollision(false);
	CreatedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	EquipedWeapon = Cast<AJT_FireWeaponBase>(CreatedWeapon);
	EquipedSlotIndex = TempQuickSlotIndex;

	EquipedWeapon->OnWeaponReloadBind.AddDynamic(this, &AJT_CharacterBase::OnWeaponReload);
}

void AJT_CharacterBase::UnequipWeapon()
{
	EquipedWeapon->OnWeaponReloadBind.RemoveDynamic(this, &AJT_CharacterBase::OnWeaponReload);
	EquipedWeapon->Destroy();
	EquipedWeapon = nullptr;
	EquipedSlotIndex = -1;
}

void AJT_CharacterBase::UseQuickSlot(int QuickSlotIndex)
{
	TempQuickSlotIndex = QuickSlotIndex;
	if (InventoryComponent->GetIsQuickSlotEmpty(QuickSlotIndex)) return;

	UJT_InventoryItemInfo* LItemInfo = InventoryComponent->QuickSlots[QuickSlotIndex]->GetFirstItem();
	if (LItemInfo->ItemClass->IsChildOf(AJT_UsableItemBase::StaticClass()))
	{
		if (AJT_UsableItemBase* LConsumableItem = Cast<AJT_UsableItemBase>(InventoryComponent->QuickSlots[QuickSlotIndex]->GetFirstItem()->ItemClass->GetDefaultObject()))
		{
			LConsumableItem->UseBy(this);
		}
	}
	TempQuickSlotIndex = -1;
}

void AJT_CharacterBase::Death()
{
	MyPlayerController->MyGameHUD->RemoveAllWidgets();
	MyPlayerController->MyGameHUD->CreateDeathMenuWD();
	MyPlayerController->UnPossess();
	Destroy();
}

float AJT_CharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeHP(-ActualDamage);

	return ActualDamage;
}

void AJT_CharacterBase::ChangeHP(float Delta)
{
	HPCurrent += Delta;
	HPCurrent = FMath::Clamp(HPCurrent, 0.0f, HPMax);
	OnHPChangeBind.Broadcast(HPCurrent, HPMax);

	if (HPCurrent == 0.0f) Death();
}



void AJT_CharacterBase::OnGameStart_Implementation()
{

	MyGameInstance->OnGameStartedBind.RemoveDynamic(this, &AJT_CharacterBase::OnGameStart);
	MyPlayerController->OnGameStart();
	FirstSpawnUpdate();
}

void AJT_CharacterBase::FirstSpawnUpdate_Implementation()
{
}

// Called when the game starts or when spawned
void AJT_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh1P->SetHiddenInGame(false, true);

	InventoryComponent->OnQuickSlotChangedBind.AddDynamic(this, &AJT_CharacterBase::OnQuickSlotChange);

	MyGameInstance = Cast<UJT_GameInstance>(GetGameInstance());

	if (IsValid(MyGameInstance))
	{
		if (MyGameInstance->GetIsGameStarted())
		{
			OnGameStart();
		}
		else
		{
			MyGameInstance->OnGameStartedBind.AddDynamic(this, &AJT_CharacterBase::OnGameStart);
		}
	}
}



// Called every frame
void AJT_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

DECLARE_DELEGATE_OneParam(FInputUseQuickSlot, int32);
// Called to bind functionality to input
void AJT_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJT_CharacterBase::OnFire);

	// Bind Inventory event
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AJT_CharacterBase::OnInventoryPressed);
	// Bind Interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AJT_CharacterBase::OnInteractPressed);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AJT_CharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJT_CharacterBase::MoveRight);

	// Bind view events
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// Bind UseQuickSlot events
	PlayerInputComponent->BindAction<FInputUseQuickSlot>("UseQuickSlot1Action", IE_Pressed, this, &AJT_CharacterBase::UseQuickSlot, 0);
	PlayerInputComponent->BindAction<FInputUseQuickSlot>("UseQuickSlot2Action", IE_Pressed, this, &AJT_CharacterBase::UseQuickSlot, 1);
	PlayerInputComponent->BindAction<FInputUseQuickSlot>("UseQuickSlot3Action", IE_Pressed, this, &AJT_CharacterBase::UseQuickSlot, 2);
	PlayerInputComponent->BindAction<FInputUseQuickSlot>("UseQuickSlot4Action", IE_Pressed, this, &AJT_CharacterBase::UseQuickSlot, 3);
	PlayerInputComponent->BindAction<FInputUseQuickSlot>("UseQuickSlot5Action", IE_Pressed, this, &AJT_CharacterBase::UseQuickSlot, 4);
}

void AJT_CharacterBase::OnFire()
{
	if (!IsValid(EquipedWeapon)) return;

	EquipedWeapon->Fire();

	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AJT_CharacterBase::OnInventoryPressed()
{
	if (IsValid(MyPlayerController) && IsValid(MyPlayerController->MyGameHUD))
		MyPlayerController->MyGameHUD->CreateInventoryWD();
}

void AJT_CharacterBase::OnInteractPressed()
{
	if (!IsValid(TargetActor)) return;

	if (TargetActor->GetClass()->ImplementsInterface(UJT_InteractiveObjectInterface::StaticClass()))
	{
		IJT_InteractiveObjectInterface::Execute_OnActivatedForInteract(TargetActor, this);
	}
}

void AJT_CharacterBase::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AJT_CharacterBase::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}





void AJT_CharacterBase::SetTargetByScreenCenter()
{
	FVector2D ScreenPosition = MyPlayerController->GetViewportHalfSize();

	FHitResult LHitRes;
	bool LIsHit = MyPlayerController->GetHitResultAtScreenRadius(ScreenPosition, ScreenTargetRadius, ECollisionChannel::ECC_Visibility, true, LHitRes);
	AActor* LTargetActor = LHitRes.Actor.Get();

	if (IsValid(LTargetActor))
	{
		AddTarget(LTargetActor);
		return;
	}

	ClearTarget();
}

void AJT_CharacterBase::UpdateTargetView()
{
	if (!IsValid(MyPlayerController)) return;

	SetTargetByScreenCenter();
}



void AJT_CharacterBase::ClearTarget()
{
	TargetActor = nullptr;
	MyPlayerController->MyGameHUD->HideInteractiveInfo();
}

void AJT_CharacterBase::AddTarget(AActor* InteractiveActor)
{
	TargetActor = InteractiveActor;

	if (TargetActor->GetClass()->ImplementsInterface(UJT_InteractiveObjectInterface::StaticClass()))
	{
		MyPlayerController->MyGameHUD->ShowInteractiveInfo(IJT_InteractiveObjectInterface::Execute_GetInteractiveMessage(TargetActor));
	}
	else
	{
		MyPlayerController->MyGameHUD->HideInteractiveInfo();
	}
}

