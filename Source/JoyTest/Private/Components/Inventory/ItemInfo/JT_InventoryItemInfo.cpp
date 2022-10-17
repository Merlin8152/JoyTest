// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"





UJT_InventoryItemInfo::UJT_InventoryItemInfo()
{
}




bool UJT_InventoryItemInfo::IsInfoDataValid_Implementation() const
{
    return IsValid(ItemClass) && IsValid(InventorySlotClass) && !ItemName.IsNone();
}

bool UJT_InventoryItemInfo::IsEqual_Implementation(UJT_InventoryItemInfo* OtherItemInfo) const
{
    if( !IsValid(OtherItemInfo) ) return false;

    return
        (
            OtherItemInfo->ItemName == ItemName &&
            OtherItemInfo->ItemClass == ItemClass &&
            OtherItemInfo->InventorySlotClass == InventorySlotClass &&
            OtherItemInfo->CanStack == CanStack &&
            OtherItemInfo->MaxInStack == MaxInStack
        );
}
