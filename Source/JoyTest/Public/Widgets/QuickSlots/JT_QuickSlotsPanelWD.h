// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_QuickSlotsPanelWD.generated.h"



class AJT_PlayerCharacter;
class UJT_QuickSlotWD;
class UHorizontalBox;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_QuickSlotsPanelWD : public UUserWidget
{
	GENERATED_BODY()


		//constructor
public:

	UJT_QuickSlotsPanelWD(const FObjectInitializer& ObjectInitializer);


	//c++ public methods
public:


	//c++ protected methods
protected:

	virtual void NativeDestruct() override;

	//c++ private methods
private:

	UFUNCTION()
		void OnQuickSlotChanged(int QuickSlotIndex);


	//c++ protected values
protected:

	int QuickSlotsCount = 0;

	//c++ private values
private:

	FTimerHandle QuickSlotsVisibilityTimer;




	//>>.......................................................................................................................................<<//



	//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "QuickSlotsPanelWD")
		void InitializeQuickSlotsWD(AJT_CharacterBase* PlayerCharacter);
	virtual void InitializeQuickSlotsWD_Implementation(AJT_CharacterBase* PlayerCharacter);



	UFUNCTION(BlueprintCallable, Category = "QuickSlotsPanelWD")
		void UpdateQuickSlots();


	//Blueprint values
public:


	/*
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "QuickSlotsPanelWD|Widgets")
		UHorizontalBox* QuickSlotsContainer = nullptr;

	/*
	Key - index of quick slot in player's inventory component.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "QuickSlotsPanelWD")
		TArray<UJT_QuickSlotWD*> QuickSlots;


	UPROPERTY(BlueprintReadOnly)
		AJT_CharacterBase* MyPlayerCharacter = nullptr;
};
