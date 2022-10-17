// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/JT_InventoryFunctionLibrary.h"

#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"

#include "Engine/SCS_Node.h"








bool UJT_InventoryFunctionLibrary::AreTwoSlotsEqual(UJT_InventorySlotBase* InventorySlotA, UJT_InventorySlotBase* InventorySlotB)
{
	if( !IsValid(InventorySlotA) || !IsValid(InventorySlotB) ) return false;

	return InventorySlotA->IsEqual(InventorySlotB);
}

bool UJT_InventoryFunctionLibrary::AreTwoItemsInfoIdentical(UJT_InventoryItemInfo* ItemInfoA, UJT_InventoryItemInfo* ItemInfoB)
{
	if( !IsValid(ItemInfoA) || !IsValid(ItemInfoB) ) return false;

	return ItemInfoA->IsEqual(ItemInfoB);
}