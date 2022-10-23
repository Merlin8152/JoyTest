// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/JT_InventoryBaseSlotWD.h"
#include "JT_InventorySlotWD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class JOYTEST_API UJT_InventorySlotWD : public UJT_InventoryBaseSlotWD
{
	GENERATED_BODY()



		// C++ public methods
public:

	virtual bool SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot) override;


	// C++ protected methods
protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;



	/*
		Validates and handles drop operation
	*/
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot) override;
	virtual bool ApplyDragDropData_Implementation(const FInventoryDragDropData& InData) override;
	/*
		Creates drag drop data with assigned slot index
	*/
	virtual void CreateDragDropData(FInventoryDragDropData& OutData) override;



	// C++  private  methods
private:


	void MoveItemToQuickSlot();


	UFUNCTION()
		void DropItemFromInventory(int ItemsCount);




	//C++ public variables
protected:

	/*
		Inventory Slot Index.
	*/
	int Index;
	/*
		Last Usable Quick Slot Index.
	*/
	int LastQuickSlotIndex = 0;

	/*
		Check Is Shift Released.
	*/
	bool IsShiftReleased = true;

	//>>>...............................................................................................................................................................................<<<//


	// Blueprint public methods
public:

	/*
		Set current Slot Inventory Index
	*/
	UFUNCTION(BlueprintCallable, Category = "InventorySlotWD")
		void SetIndex(int32 InIndex);

	
};
