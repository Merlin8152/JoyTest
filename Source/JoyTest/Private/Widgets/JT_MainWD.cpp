// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JT_MainWD.h"

#include "Character/JT_CharacterBase.h"
#include "Widgets/QuickSlots/JT_QuickSlotsPanelWD.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "GameFramework/InputSettings.h"

UJT_MainWD::UJT_MainWD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UJT_MainWD::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsValid(MyPlayerCharacter)) return;

	MyPlayerCharacter->OnHPChangeBind.RemoveDynamic(this, &UJT_MainWD::UpdateHP);
}

void UJT_MainWD::InitializeMainWD_Implementation(AJT_CharacterBase* PlayerCharacter)
{
	MyPlayerCharacter = PlayerCharacter;

	if (!IsValid(MyPlayerCharacter)) return;


	MyPlayerCharacter->OnHPChangeBind.AddDynamic(this, &UJT_MainWD::UpdateHP);
	UpdateHP(MyPlayerCharacter->GetHP(), MyPlayerCharacter->GetHPMax());

	if (QuickSlotsPanel) QuickSlotsPanel->InitializeQuickSlotsWD(MyPlayerCharacter);
}

void UJT_MainWD::UpdateHP(float NewHP, float MaxHP)
{
	if (HPBar) HPBar->SetPercent(NewHP / MaxHP);
}

void UJT_MainWD::SetInteractiveInfo(const FText& InfoText)
{
	if (InteractiveInfoText)
	{
		TArray<FInputActionKeyMapping> LOutMappings;
		UInputSettings::GetInputSettings()->GetActionMappingByName("Interact", LOutMappings);


		FText OutText = FText::FromString(LOutMappings[0].Key.ToString() + ": " + InfoText.ToString() + "");
		InteractiveInfoText->SetVisibility(ESlateVisibility::Visible);
		InteractiveInfoText->SetText(OutText);
	}
}

void UJT_MainWD::HideInteractiveInfo()
{
	if (InteractiveInfoText)
	{
		InteractiveInfoText->SetVisibility(ESlateVisibility::Collapsed);
		InteractiveInfoText->SetText(FText::GetEmpty());
	}
}
