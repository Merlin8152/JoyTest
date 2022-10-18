// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/JT_InventoryBaseSlotWD.h"
#include "JT_InventoryQuickSlotWD.generated.h"

/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_InventoryQuickSlotWD : public UJT_InventoryBaseSlotWD
{
	GENERATED_BODY()



		// C++ public methods
public:

	virtual bool HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot) override;
	virtual bool ApplyDragDropData_Implementation(const FInventoryDragDropData& InData) override;



	virtual bool SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot) override;


protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);



	void MoveSlotToInventory();



	/** Handles drag drop operation */
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	/** Performs unequipping item from equipment slot */
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/** Creates Drag Drop data with assigned equipment slot type */
	virtual void CreateDragDropData(FInventoryDragDropData& OutData) override;


	// C++ public variables
public:

	/*
		Index of QuickSlot
	*/
	int QuickSlotIndex;




	//>>>...............................................................................................................................................................................<<<//
	//>>>...............................................................................................................................................................................<<<//




	// Blueprint variables
protected:

	/*
		Texture for Quick Slot Icon
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuickSlotWD")
		UTexture2D* QuickSlotIconTexture = nullptr;


	// Blueprint public methods
public:



};
