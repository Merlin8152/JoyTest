// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_QuickSlotsConteinerWD.generated.h"

class UJT_InventoryQuickSlotWD;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_QuickSlotsConteinerWD : public UUserWidget
{
	GENERATED_BODY()

		//C++ public methods
public:
	TArray<UJT_InventoryQuickSlotWD*> GetQuickSlotWidgets();
};
