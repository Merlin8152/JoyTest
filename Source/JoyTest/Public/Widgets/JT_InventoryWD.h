// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_InventoryWD.generated.h"

class UJT_InventoryItemInfo;
class UJT_InventorySlotWD;
class UJT_InventoryComponent;
class AJT_GameHUD;
class AJT_CharacterBase;

class UJT_InventoryBaseSlotWD;
class UJT_QuickSlotsConteinerWD;
class UJT_InventoryQuickSlotWD;

class UUniformGridPanel;
class UButton;
class UTextBlock;
class UImage;
class UHorizontalBox;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_InventoryWD : public UUserWidget
{
	GENERATED_BODY()

		//Constructor
public:
	UJT_InventoryWD(const FObjectInitializer& ObjectInitializer);

	//C++ protected methods
protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


	UFUNCTION()
		void OnCloseButtonClicked();

	/*
	List of Droppted Items.
	*/
	UPROPERTY()
		TArray<UJT_InventoryItemInfo*> ItemToDrop;


	//..............................................InventorySlots................................................................//

	/*
		Create Inventory Slots and fill Inventory panel.
	*/
	UFUNCTION()
		void GenerateInventorySlots();
	/*
		fill Inventory panel (for update inventory category).
	*/
	void UpdateInventorySlots();
	/*
		Create Inventory Slot.
	*/
	UJT_InventorySlotWD* CreateInventorySlot(int32 InIndex);
	/*
		Setup Inventory Slot in Inventory panel.
	*/
	void SetInventorySlotData(UJT_InventoryItemInfo* ItemInfo, int Index, int& NextValidIndex);

	//................................................................................................................................//

//Blueprint public methods
public:


	//..............................................Initializes................................................................//

	/*
		Initializes Inventory Widget
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventoryWD")
		bool InitializeInventoryWD(UJT_InventoryComponent* InInventoryComponent, AJT_GameHUD* NewGameHUD);
	bool InitializeInventoryWD_Implementation(UJT_InventoryComponent* InInventoryComponent, AJT_GameHUD* NewGameHUD);

	UFUNCTION()
		void GenerateQuickSlots();
	UFUNCTION()
		void InitializeQuickSlot(int SlotIndex);

	//................................................................................................................................//

	//Blueprint variables
public:

	/*
		MaxColumns in Inventory.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryWD | Settings")
		int MaxColumns = 5;


	/*
	InventoryPanel - Panel for displaying items in inventory.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "InventoryWD|Widgets")
		UUniformGridPanel* InventoryPanel = nullptr;
	/*
		Class of Inventory Slot in panel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryWD|Classes")
		TSubclassOf<UJT_InventoryBaseSlotWD> InventorySlotClass = nullptr;

	/*
		Container for Quick slots WD.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "InventoryWD|Widgets")
		UJT_QuickSlotsConteinerWD* QuickSlotContainer = nullptr;
	/*
	Button for close inventory.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "InventoryWD|Widgets")
		UButton* CloseButton = nullptr;



	/*LinksAndComponents*/

	UPROPERTY(BlueprintReadOnly)
		UJT_InventoryComponent* InventoryComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AJT_CharacterBase* Player = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AJT_GameHUD* MyGameHUD = nullptr;

	/*~LinksAndComponents*/
};
