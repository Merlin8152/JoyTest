// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/QuickSlots/JT_QuickSlotWD.h"


#include "Components/TextBlock.h"
#include "Components/Image.h"


#include "GameFramework/InputSettings.h"




UJT_QuickSlotWD::UJT_QuickSlotWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}



void UJT_QuickSlotWD::InitializeQuickSlotWD_Implementation(UTexture2D* IconTexture, int Count)
{
	if (IsValid(IconImage))
	{
		IconImage->SetBrushFromTexture(IconTexture);
		IconImage->SetOpacity(IsValid(IconTexture));
	}
	else 
	{
		IconImage->SetOpacity(0);
	}


	if (IsValid(HotKeyText))
	{
		TArray<FInputActionKeyMapping> LOutMappings;
		UInputSettings::GetInputSettings()->GetActionMappingByName(HotKeyName, LOutMappings);

		HotKeyText->SetText(LOutMappings[0].Key.GetDisplayName());
		HotKeyText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HotKeyText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(CountText) && Count > 1)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Count)));
		CountText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UJT_QuickSlotWD::ClearQuickSlot_Implementation()
{
	if (HotKeyText) HotKeyText->SetVisibility(ESlateVisibility::Collapsed);
	if (IconImage) IconImage->SetOpacity(0);
	if (CountText) HotKeyText->SetVisibility(ESlateVisibility::Collapsed);
}