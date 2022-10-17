// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DragDrop/JT_InventoryDragDropOperation.h"

bool UJT_InventoryDragDropOperation::InitializesDragDropData_Implementation(const FInventoryDragDropData& InDragDropData)
{
	Data = InDragDropData;
	return IsValid(Data.SlotWidget);
}

FInventoryDragDropData UJT_InventoryDragDropOperation::GetData_Implementation() const
{
	return Data;
}
