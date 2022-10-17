// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Blueprint/UserWidget.h"
#include "JT_DragDropStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryDragDropData
{
	GENERATED_BODY()

		FInventoryDragDropData() {}


	/** Dragged item */
	UPROPERTY(EditAnywhere, Category = "Inventory Drag Drop Data")
		UJT_InventoryItemInfo* ItemInfo = nullptr;

	/** Dragged slot widget */
	UPROPERTY(EditAnywhere, Category = "Inventory Drag Drop Data")
		UUserWidget* SlotWidget = nullptr;


	/** Dragged inventory slot index */
	UPROPERTY(EditAnywhere, Category = "Inventory Drag Drop Data")
		int SlotIndex = -1;


	/** Dragged inventory Quick slot index */
	UPROPERTY(EditAnywhere, Category = "Inventory Drag Drop Data")
		int QuickSlotIndex = -1;


	FORCEINLINE bool IsFromQuickSlot() const
	{
		return QuickSlotIndex != -1;
	}

};