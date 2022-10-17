// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JT_InventoryWD.h"


#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Components/Inventory/JT_InventoryComponent.h"
#include "Character/JT_CharacterBase.h"

#include "Widgets/Inventory/JT_InventoryBaseSlotWD.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "Widgets/Inventory/JT_InventorySlotWD.h"
#include "Widgets/Inventory/JT_QuickSlotsConteinerWD.h"
#include "Widgets/Inventory/JT_InventoryQuickSlotWD.h"

#include "GameObjects/JT_PlayerController.h"
#include "GameObjects/JT_GameHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"



UJT_InventoryWD::UJT_InventoryWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UJT_InventoryWD::NativeConstruct()
{
}

void UJT_InventoryWD::NativeDestruct()
{
}

void UJT_InventoryWD::OnCloseButtonClicked()
{
	if (IsValid(MyGameHUD)) MyGameHUD->RemoveInventoryWD();
}

void UJT_InventoryWD::GenerateInventorySlots()
{
	if (!IsValid(InventoryPanel) || !IsValid(InventorySlotClass)) return;

	InventoryPanel->ClearChildren();

	int LNextValidIndex = 0;

	for (int Index = 0; Index < InventoryComponent->Inventory.Num(); Index++)
	{
		if (InventoryComponent->Inventory.IsValidIndex(Index))
		{
			CreateInventorySlot(Index);

			const UJT_InventorySlotBase* LInventorySlot = InventoryComponent->Inventory[Index];

			SetInventorySlotData(LInventorySlot->GetFirstItem(), Index, LNextValidIndex);
		}
	}
}

void UJT_InventoryWD::UpdateInventorySlots()
{
	if (!IsValid(InventoryPanel) || !IsValid(InventorySlotClass)) return;

	InventoryPanel->ClearChildren();

	int LNextValidIndex = 0;

	for (int Index = 0; Index < InventoryComponent->Inventory.Num(); Index++)
	{
		if (InventoryComponent->Inventory.IsValidIndex(Index))
		{
			const auto LInventorySlot = InventoryComponent->Inventory[Index];

			SetInventorySlotData(LInventorySlot->GetFirstItem(), Index, LNextValidIndex);
		}
	}
}



UJT_InventorySlotWD* UJT_InventoryWD::CreateInventorySlot(int32 InIndex)
{
	UJT_InventorySlotWD* LInventorySlot = CreateWidget<UJT_InventorySlotWD>(GetOwningPlayer(), InventorySlotClass);

	LInventorySlot->SetIndex(InIndex);

	InventoryPanel->AddChildToUniformGrid(LInventorySlot);

	return LInventorySlot;
}


void UJT_InventoryWD::SetInventorySlotData(UJT_InventoryItemInfo* ItemInfo, int Index, int& NextValidIndex)
{
	bool bLValidItem = IsValid(ItemInfo) && ItemInfo->IsInfoDataValid();

	UWidget* LCurrentSlotWidget = InventoryPanel->GetChildAt(Index);

	if (!IsValid(LCurrentSlotWidget))
	{
		LCurrentSlotWidget = CreateInventorySlot(Index);
	}

	UJT_InventoryItemInfo* LInventoryItemInfo = bLValidItem ? ItemInfo : NewObject<UJT_InventoryItemInfo>();

	UJT_InventorySlotWD* LInventorySlotWD = Cast<UJT_InventorySlotWD>(LCurrentSlotWidget);

	if (!IsValid(LInventorySlotWD)) return;

	LInventorySlotWD->SetIndex(Index);

	LInventorySlotWD->InitializesBaseSlot(this, InventoryComponent, MyGameHUD);

	LInventorySlotWD->SetNewItemInfo(LInventoryItemInfo, true);

	UUniformGridSlot* LGridSlotWidget = UWidgetLayoutLibrary::SlotAsUniformGridSlot(LCurrentSlotWidget);

	int LRow = -1;
	int LColumn = -1;

	LRow = NextValidIndex / MaxColumns;
	LColumn = NextValidIndex % MaxColumns;

	LGridSlotWidget->SetRow(LRow);
	LGridSlotWidget->SetColumn(LColumn);

	LCurrentSlotWidget->SetVisibility(ESlateVisibility::Visible);
	NextValidIndex++;

}

bool UJT_InventoryWD::InitializeInventoryWD_Implementation(UJT_InventoryComponent* InInventoryComponent, AJT_GameHUD* NewGameHUD)
{
	if (!IsValid(InInventoryComponent) || !IsValid(NewGameHUD)) return false;

	InventoryComponent = InInventoryComponent;

	MyGameHUD = NewGameHUD;
	Player = MyGameHUD->MyController->MyCharacter;

	if (CloseButton) CloseButton->OnClicked.AddDynamic(this, &UJT_InventoryWD::OnCloseButtonClicked);

	GenerateInventorySlots();

	GenerateQuickSlots();

	InventoryComponent->OnInventoryChangedBind.AddDynamic(this, &UJT_InventoryWD::GenerateInventorySlots);
	InventoryComponent->OnQuickSlotChangedBind.AddDynamic(this, &UJT_InventoryWD::InitializeQuickSlot);

	return true;
}

void UJT_InventoryWD::GenerateQuickSlots()
{
	if (!IsValid(QuickSlotContainer)) return;

	TArray<UJT_InventoryQuickSlotWD*> QuickSlots = QuickSlotContainer->GetQuickSlotWidgets();

	int i = 0;

	for (UJT_InventoryQuickSlotWD* LQuickSlotWD : QuickSlots)
	{
		if (!IsValid(LQuickSlotWD) || !InventoryComponent->QuickSlots.IsValidIndex(i)) return;

		UJT_InventoryItemInfo* LQuickSlotData = InventoryComponent->QuickSlots[i]->GetFirstItem();

		LQuickSlotWD->QuickSlotIndex = i;
		LQuickSlotWD->InitializesBaseSlot(this, InventoryComponent, MyGameHUD);
		LQuickSlotWD->SetNewItemInfo(LQuickSlotData, true); /// Must be after initialization

		i++;
	}
}

void UJT_InventoryWD::InitializeQuickSlot(int SlotIndex)
{
	if (!IsValid(QuickSlotContainer) || !InventoryComponent->QuickSlots.IsValidIndex(SlotIndex)) return;

	TArray<UJT_InventoryQuickSlotWD*> QuickSlots = QuickSlotContainer->GetQuickSlotWidgets();

	if (QuickSlots.IsValidIndex(SlotIndex))
	{
		UJT_InventoryQuickSlotWD* LQuickSlotWidget = Cast<UJT_InventoryQuickSlotWD>(QuickSlots[SlotIndex]);

		if (!IsValid(LQuickSlotWidget)) return;

		const auto QuickSlotData = InventoryComponent->QuickSlots[SlotIndex]->GetFirstItem();

		LQuickSlotWidget->QuickSlotIndex = SlotIndex;
		LQuickSlotWidget->InitializesBaseSlot(this, InventoryComponent, MyGameHUD);
		LQuickSlotWidget->SetNewItemInfo(QuickSlotData, true); /// Must be after initialization
	}
}


