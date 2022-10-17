// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/JT_QuickSlotsConteinerWD.h"

#include "Widgets/Inventory/JT_InventoryQuickSlotWD.h"

#include "Blueprint/WidgetTree.h"

TArray<UJT_InventoryQuickSlotWD*> UJT_QuickSlotsConteinerWD::GetQuickSlotWidgets()
{
	TArray<UJT_InventoryQuickSlotWD*> QuickSlotWidgets;

	WidgetTree->ForEachWidget(
		[&](UWidget* Widget)
		{
			if (UJT_InventoryQuickSlotWD* QuickSlot = Cast<UJT_InventoryQuickSlotWD>(Widget))
			{
				QuickSlotWidgets.Add(QuickSlot);
			}
		});


	return QuickSlotWidgets;
}
