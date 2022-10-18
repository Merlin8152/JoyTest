// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_QuickSlotWD.generated.h"


class UImage;
class UTextBlock;

/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_QuickSlotWD : public UUserWidget
{
	GENERATED_BODY()

		UJT_QuickSlotWD(const FObjectInitializer& ObjectInitializer);




	//>>.............................................................................................................<<//



	//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "QuickSlotWD")
		void InitializeQuickSlotWD(UTexture2D* IconTexture, int Count = 0);
	virtual void InitializeQuickSlotWD_Implementation(UTexture2D* IconTexture, int Count = 0);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "QuickSlotWD")
		void ClearQuickSlot();
	virtual void ClearQuickSlot_Implementation();



	//Blueprint values
public:

	/*
		Confirmation text for close setting window if we have uncommitted changes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true), Category = "SettingsWD")
		FName HotKeyName;

	/*
		Image for item icon in quick slot.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "QuickSlotWD|Widgets")
		UImage* IconImage = nullptr;
	/*
		Text block for items count in quick slot.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "QuickSlotWD|Widgets")
		UTextBlock* HotKeyText = nullptr;

	/*
		Text block for items count in quick slot.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "QuickSlotWD|Widgets")
		UTextBlock* CountText = nullptr;

};
