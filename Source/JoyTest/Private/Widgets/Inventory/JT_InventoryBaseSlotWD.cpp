// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/JT_InventoryBaseSlotWD.h"


#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Widgets/Inventory/DragDrop/JT_DragDropSlotWD.h"
#include "Widgets/JT_InventoryWD.h"
#include "GameObjects/JT_GameHUD.h"
#include "Components/Inventory/JT_InventoryComponent.h"
#include "Widgets/Inventory/DragDrop/JT_InventoryDragDropOperation.h"


#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/InputSettings.h"

UJT_InventoryBaseSlotWD::UJT_InventoryBaseSlotWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


bool UJT_InventoryBaseSlotWD::SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot)
{
	if (!IsValid(ItemIcon)) return false;

	if (!IsValid(NewItemInfo))
	{
		ItemIcon->SetOpacity(0);
		return false;
	}

	ItemInfo = NewItemInfo;

	if (!ItemInfo->Icon)
	{
		ItemIcon->SetOpacity(0);
	}
	else
	{
		ItemIcon->SetOpacity(1);
		ItemIcon->SetBrushFromTexture(ItemInfo->Icon);
	}

	if (bUpdateSlot) UpdateSlotInfo();

	return true;
}

void UJT_InventoryBaseSlotWD::NativeDestruct()
{
	UWidgetBlueprintLibrary::CancelDragDrop(); /// Remove Drag n drop widget
}

FReply UJT_InventoryBaseSlotWD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsSlotFree())
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
	else
	{
		const FEventReply LEventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey(EKeys::LeftMouseButton));

		return LEventReply.NativeReply;
	}
}

void UJT_InventoryBaseSlotWD::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//TODO
}

void UJT_InventoryBaseSlotWD::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (IsValid(InventoryWD)) InventoryWD->SetFocus();
}

FReply UJT_InventoryBaseSlotWD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FReply Reply = FReply::Unhandled();

	if (!IsValid(InventoryWD)) return Reply;

	TArray<FInputActionKeyMapping> LOutMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Inventory", LOutMappings);

	if (InKeyEvent.GetKey() == LOutMappings[0].Key)
	{
		GameHUD->RemoveInventoryWD();

		Reply = FReply::Handled();
	}
	else if (InKeyEvent.GetKey() == FKey(EKeys::Escape))
	{
		GameHUD->RemoveInventoryWD();

		Reply = FReply::Handled();
	}


	return Reply;
}

void UJT_InventoryBaseSlotWD::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	OutOperation = CreateDragDropOperation();

	if (!IsValid(OutOperation) || !IsValid(DragDropVisualClass)) return;

	UUserWidget* LDragVisual = nullptr;

	const auto Owner = GetOwningPlayer();
	const auto WidgetName = FName("Inventory Drag Visual");
	LDragVisual = CreateWidget(Owner, DragDropVisualClass, WidgetName);

	if (auto DragVisualSlot = Cast<UJT_DragDropSlotWD>(LDragVisual))
	{
		DragVisualSlot->DragDropSlotIcon->SetBrushFromTexture(ItemInfo->Icon);
	}

	OutOperation->DefaultDragVisual = LDragVisual;
}

void UJT_InventoryBaseSlotWD::InitializesBaseSlot(UJT_InventoryWD* NewInventoryWD, UJT_InventoryComponent* NewInventoryComponent, AJT_GameHUD* NewGameHUD)
{
	if (!IsValid(NewInventoryWD) || !IsValid(NewInventoryComponent) || !IsValid(NewGameHUD)) return;

	GameHUD = NewGameHUD;
	InventoryWD = NewInventoryWD;
	InventoryComponent = NewInventoryComponent;

	UpdateSlotInfo();
}

bool UJT_InventoryBaseSlotWD::HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot)
{
	const auto InventoryOperation = Cast<UJT_InventoryDragDropOperation>(Operation);
	if (!IsValid(InventoryOperation)) return false;

	const auto LData = InventoryOperation->GetData();

	return SetNewItemInfo(LData.ItemInfo, bUpdateSlot);
}

bool UJT_InventoryBaseSlotWD::ApplyDragDropData_Implementation(const FInventoryDragDropData& InData)
{
	this->SetFocus();

	return true;
}

void UJT_InventoryBaseSlotWD::CreateDragDropData(FInventoryDragDropData& OutData)
{
	OutData = FInventoryDragDropData();
	OutData.ItemInfo = ItemInfo;

	OutData.SlotWidget = this;

	if (!IsValid(InventoryWD)) return;
}

UDragDropOperation* UJT_InventoryBaseSlotWD::CreateDragDropOperation()
{
	UDragDropOperation* LOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UJT_InventoryDragDropOperation::StaticClass());

	UJT_InventoryDragDropOperation* InventoryOperation = Cast<UJT_InventoryDragDropOperation>(LOperation);

	if (!IsValid(InventoryOperation)) return nullptr;

	FInventoryDragDropData LDragDropData;
	CreateDragDropData(LDragDropData);

	const bool bLSuccess = InventoryOperation->InitializesDragDropData(LDragDropData);
	return bLSuccess ? LOperation : nullptr;
}

void UJT_InventoryBaseSlotWD::ClearSlotInfo_Implementation()
{
	ItemInfo = NewObject<UJT_InventoryItemInfo>();
	if (IsValid(ItemCount)) ItemCount->SetOpacity(0);

	UpdateSlotInfo();
}

void UJT_InventoryBaseSlotWD::UpdateSlotInfo_Implementation()
{
	if (!IsValid(ItemInfo) || !IsValid(ItemIcon) || !IsValid(InventoryWD)) return;


	if (!ItemInfo->Icon)
	{
		ItemIcon->SetOpacity(0);
	}
	else
	{
		ItemIcon->SetOpacity(1);
		ItemIcon->SetBrushFromTexture(ItemInfo->Icon);
	}
}


bool UJT_InventoryBaseSlotWD::IsSlotFree() const
{
	if (!IsValid(ItemInfo)) return true;

	return ItemInfo->ItemName.IsNone();
}
