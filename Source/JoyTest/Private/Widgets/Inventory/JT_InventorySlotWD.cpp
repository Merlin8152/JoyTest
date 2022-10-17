// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/JT_InventorySlotWD.h"

#include "Components/Inventory/JT_InventoryComponent.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "Components/TextBlock.h"
#include "Widgets/Inventory/DragDrop/JT_InventoryDragDropOperation.h"
#include "InteractiveObjects/PickUpItems/UsableItems/JT_UsableItemBase.h"
#include "Widgets/JT_InventoryWD.h"
#include "GameObjects/JT_GameHUD.h"

void UJT_InventorySlotWD::SetIndex(int32 InIndex)
{
	Index = InIndex;
}

bool UJT_InventorySlotWD::SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot)
{
	Super::SetNewItemInfo(NewItemInfo, bUpdateSlot);

	if (IsValid(InventoryComponent) && InventoryComponent->Inventory.IsValidIndex(Index) && !(InventoryComponent->Inventory[Index]->IsEmpty()) && InventoryComponent->Inventory[Index]->GetFirstItem()->CanStack)
	{
		ItemCount->SetOpacity(1);
		ItemCount->SetText(FText::AsNumber(InventoryComponent->Inventory[Index]->GetItemsCount()));
	}

	return true;
}

void UJT_InventorySlotWD::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UJT_InventorySlotWD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FReply Reply = FReply::Unhandled();

	if (!IsSlotFree())
	{
		if (InKeyEvent.GetKey() == FKey(EKeys::E))
		{
			MoveItemToQuickSlot();

			Reply = FReply::Handled();
		}
		else if (InKeyEvent.GetKey() == FKey(EKeys::R))
		{
			DropItemFromInventory(InventoryComponent->Inventory[Index]->GetItemsCount());

			Reply = FReply::Handled();
		}
	}


	return Reply;
}

FReply UJT_InventorySlotWD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Unhandled();

	Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!IsValid(InventoryWD) || IsSlotFree()) return Reply;

	if (InMouseEvent.GetEffectingButton().GetFName() == FKey(EKeys::RightMouseButton))
	{
		MoveItemToQuickSlot();

		Reply = FReply::Handled();
	}

	return Reply;
}

void UJT_InventorySlotWD::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

}

void UJT_InventorySlotWD::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

bool UJT_InventorySlotWD::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return HandleDragDrop(InOperation, true);
}

bool UJT_InventorySlotWD::HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot)
{
	const UJT_InventoryDragDropOperation* InventoryOperation = Cast<UJT_InventoryDragDropOperation>(Operation);

	if (!IsValid(InventoryOperation)) return false;

	FInventoryDragDropData LData = InventoryOperation->GetData();

	FInventoryDragDropData Data;
	CreateDragDropData(Data);

	UJT_InventoryBaseSlotWD* SlotWidget = Cast<UJT_InventoryBaseSlotWD>(LData.SlotWidget);

	return SlotWidget->ApplyDragDropData(Data);
}

void UJT_InventorySlotWD::CreateDragDropData(FInventoryDragDropData& OutData)
{
	Super::CreateDragDropData(OutData);
	OutData.SlotIndex = Index;
}

void UJT_InventorySlotWD::MoveItemToQuickSlot()
{
	if (!IsValid(InventoryComponent) || !IsValid(GameHUD) || !IsValid(ItemInfo)) return;


	int i = 0;
	int LItemCount = InventoryComponent->Inventory[Index]->GetItemsCount();


	for (auto QuickSlot : InventoryComponent->QuickSlots)
	{
		if (QuickSlot->IsEmpty() || QuickSlot->GetCanAddItem(ItemInfo))
		{
			InventoryComponent->MoveItemsFromInventoryToQuickSlot(Index, i, LItemCount);

			LastQuickSlotIndex = i;
			return;
		}

		i++;
	}

	if (InventoryComponent->QuickSlots.IsValidIndex(LastQuickSlotIndex))
	{
		InventoryComponent->QuickSlots[LastQuickSlotIndex]->MoveLastItemsCountToInventory(LItemCount, false, InventoryComponent);
		InventoryComponent->MoveItemsFromInventoryToQuickSlot(Index, LastQuickSlotIndex, LItemCount);
	}
}


void UJT_InventorySlotWD::DropItemFromInventory(int ItemsCount)
{
	if (!IsValid(InventoryWD) || !IsValid(InventoryComponent) || !IsValid(GameHUD)) return;

	InventoryComponent->RemoveFromInventory(Index, ItemsCount);
}
