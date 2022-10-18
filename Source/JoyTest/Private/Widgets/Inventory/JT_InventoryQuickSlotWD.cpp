// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/JT_InventoryQuickSlotWD.h"

#include "Widgets/JT_InventoryWD.h"
#include "GameObjects/JT_GameHUD.h"
#include "Components/Inventory/JT_InventoryComponent.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "InteractiveObjects/PickUpItems/UsableItems/JT_UsableItemBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Widgets/Inventory/DragDrop/JT_InventoryDragDropOperation.h"

void UJT_InventoryQuickSlotWD::NativeConstruct()
{
	Super::NativeConstruct();
}


//..................................................................DragDrop.............................................................................//


bool UJT_InventoryQuickSlotWD::ApplyDragDropData_Implementation(const FInventoryDragDropData& InData)
{
	auto a = InData.ItemInfo->ItemClass->ClassConstructor;
	if (InData.SlotWidget == this || !IsValid(InventoryWD)) return false;



	UJT_InventoryBaseSlotWD* FromSlotWidget = Cast<UJT_InventoryBaseSlotWD>(InData.SlotWidget);
	FromSlotWidget->ClearSlotInfo();

	if (InData.IsFromQuickSlot())
	{
		UJT_InventoryItemInfo* FromItemInfo = InventoryComponent->QuickSlots[InData.QuickSlotIndex]->GetFirstItem();
		int count = InventoryComponent->QuickSlots[InData.QuickSlotIndex]->GetItemsCount();

		if (!IsSlotFree())
		{
			InventoryComponent->SetQuickSlotByItemInfo(InData.QuickSlotIndex, ItemInfo);
			FromSlotWidget->SetNewItemInfo(ItemInfo, true);
		}

		SetNewItemInfo(FromSlotWidget->ItemInfo, true);

		InventoryComponent->MoveItemsCountFromQuickSlotToQuickSlot(InData.QuickSlotIndex, QuickSlotIndex, count, true);
	}
	else
	{
		InventoryComponent->MoveItemsFromInventoryToQuickSlot(InData.SlotIndex, QuickSlotIndex, InventoryComponent->Inventory[InData.SlotIndex]->GetItemsCount());
	}


	FromSlotWidget->UpdateSlotInfo();

	return true;
}







void UJT_InventoryQuickSlotWD::CreateDragDropData(FInventoryDragDropData& OutData)
{
	Super::CreateDragDropData(OutData);
	OutData.QuickSlotIndex = QuickSlotIndex;
}





bool UJT_InventoryQuickSlotWD::HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot)
{
	if (!IsValid(Operation)) return false;

	const UJT_InventoryDragDropOperation* InventoryOperation = Cast<UJT_InventoryDragDropOperation>(Operation);

	if (!IsValid(InventoryOperation)) return false;

	const FInventoryDragDropData LData = InventoryOperation->GetData();

	return ApplyDragDropData(LData);
}





bool UJT_InventoryQuickSlotWD::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return HandleDragDrop(InOperation, true);
}



void UJT_InventoryQuickSlotWD::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	MoveSlotToInventory();
}






//..........................................................................................................................................................//





FReply UJT_InventoryQuickSlotWD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FReply Reply = FReply::Unhandled();

	if (!IsValid(InventoryWD) || IsSlotFree() || !IsValid(InventoryComponent) || !IsValid(GameHUD)) return Reply;

	if (InKeyEvent.GetKey() == FKey(EKeys::E))
	{
		MoveSlotToInventory();

		Reply = FReply::Handled();
	}

	return Reply;
}

FReply UJT_InventoryQuickSlotWD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Unhandled();

	Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	if (!IsValid(InventoryWD) || IsSlotFree()) return Reply;

	if (InMouseEvent.GetEffectingButton().GetFName() == FKey(EKeys::RightMouseButton))
	{
		MoveSlotToInventory();

		ClearSlotInfo();

		Reply = FReply::Handled();
	}

	return Reply;
}







bool UJT_InventoryQuickSlotWD::SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot)
{
	Super::SetNewItemInfo(NewItemInfo, bUpdateSlot);

	if (!IsValid(ItemCount) || !InventoryComponent->QuickSlots.IsValidIndex(QuickSlotIndex)) return false;

	if (IsValid(InventoryComponent) && InventoryComponent->QuickSlots[QuickSlotIndex]->GetItemsCount() > 1)
	{
		ItemCount->SetOpacity(1);
		ItemCount->SetText(FText::AsNumber(InventoryComponent->QuickSlots[QuickSlotIndex]->GetItemsCount()));
	}
	else
	{
		ItemCount->SetOpacity(0);
	}

	return true;
}






void UJT_InventoryQuickSlotWD::MoveSlotToInventory()
{
	if (!IsValid(InventoryComponent)) return;

	const int SlotItemCount = InventoryComponent->QuickSlots[QuickSlotIndex]->GetItemsCount();

	InventoryComponent->MoveItemsFromQuickSlotToInventory(QuickSlotIndex, SlotItemCount, false, true);
	//InventoryComponent->QuickSlots[QuickSlotIndex]->MoveLastItemsCountToInventory(SlotItemCount, false, InventoryComponent);

	ClearSlotInfo();
}



