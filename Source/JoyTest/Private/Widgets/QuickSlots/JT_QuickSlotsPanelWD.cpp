// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/QuickSlots/JT_QuickSlotsPanelWD.h"

#include "Widgets/QuickSlots/JT_QuickSlotWD.h"

#include "Character/JT_CharacterBase.h"
#include "Components/Inventory/JT_InventoryComponent.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

//..................................................//

#include "Blueprint/WidgetTree.h"

UJT_QuickSlotsPanelWD::UJT_QuickSlotsPanelWD(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UJT_QuickSlotsPanelWD::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsValid(MyPlayerCharacter)) return;


	MyPlayerCharacter->GetInventoryComponent()->OnQuickSlotChangedBind.RemoveDynamic(this, &UJT_QuickSlotsPanelWD::OnQuickSlotChanged);
}

void UJT_QuickSlotsPanelWD::InitializeQuickSlotsWD_Implementation(AJT_CharacterBase* PlayerCharacter)
{
	MyPlayerCharacter = PlayerCharacter;

	if (!IsValid(MyPlayerCharacter)) return;


	QuickSlotsCount = MyPlayerCharacter->GetInventoryComponent()->QuickSlots.Num();

	MyPlayerCharacter->GetInventoryComponent()->OnQuickSlotChangedBind.AddDynamic(this, &UJT_QuickSlotsPanelWD::OnQuickSlotChanged);

	UpdateQuickSlots();
}

void UJT_QuickSlotsPanelWD::UpdateQuickSlots()
{
	if (!IsValid(QuickSlotsContainer)) return;

	QuickSlots.Empty();

	WidgetTree->ForEachWidget([&](UWidget* Widget)
		{
			if (UJT_QuickSlotWD* QuickSlotWD = Cast<UJT_QuickSlotWD>(Widget))
			{
				QuickSlotWD->InitializeQuickSlotWD(nullptr);
				QuickSlots.Add(QuickSlotWD);
			}
		});
}


void UJT_QuickSlotsPanelWD::OnQuickSlotChanged(int QuickSlotIndex)
{
	if (!IsValid(MyPlayerCharacter) || !QuickSlots.IsValidIndex(QuickSlotIndex)
		|| !MyPlayerCharacter->GetInventoryComponent()->QuickSlots.IsValidIndex(QuickSlotIndex)) return;

	if (MyPlayerCharacter->GetInventoryComponent()->QuickSlots[QuickSlotIndex]->IsEmpty())
	{
		QuickSlots[QuickSlotIndex]->InitializeQuickSlotWD(nullptr);
	}
	else
	{
		auto InvSlot = MyPlayerCharacter->GetInventoryComponent()->QuickSlots[QuickSlotIndex];
		int Count = MyPlayerCharacter->GetInventoryComponent()->QuickSlots[QuickSlotIndex]->GetItemsCount();
		auto ItemInfo = InvSlot->GetFirstItem();
		UTexture2D* Icon = ItemInfo->Icon;
		QuickSlots[QuickSlotIndex]->InitializeQuickSlotWD(Icon, Count);
	}
}
