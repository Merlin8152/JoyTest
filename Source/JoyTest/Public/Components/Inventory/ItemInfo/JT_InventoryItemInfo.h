// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JT_InventoryItemInfo.generated.h"




class UJT_InventorySlotBase;



/*
	Information about item in inventory.
*/
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class JOYTEST_API UJT_InventoryItemInfo : public UObject
{
	GENERATED_BODY()


//constructor
public:

	UJT_InventoryItemInfo();

	



//c++ public methods
public:



//>>.................................................................................................................................<<//



//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryItemInfo")
		bool IsInfoDataValid() const;
		virtual bool IsInfoDataValid_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryItemInfo")
		bool IsEqual(UJT_InventoryItemInfo* OtherItemInfo) const;
		virtual bool IsEqual_Implementation(UJT_InventoryItemInfo* OtherItemInfo) const;



//Blueprint values
public:

	/* 
		What will happen after the dropping (usually the original class) + is the "face" of the subject. This class is checked on CanBeEquiptToSlot.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		TSubclassOf<AActor> ItemClass = nullptr;

	/* 
		When adding it, it will search for this slot or create it if everyone is busy.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		TSubclassOf<UJT_InventorySlotBase> InventorySlotClass = nullptr;

	/*
		Name of the item for the system.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		FName ItemName = NAME_None;
	/*
		Name of the item to display.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		FText DisplayName = FText::FromString("None");

	/*
		Item Icon.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		UTexture2D* Icon = nullptr;

	/*
		Item Description.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true), Category = "InventoryItemInfo")
		FText Description;


	/*
		Can it stack.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItemInfo")
		bool CanStack = false;
	/*
		The maximum number of these items in one machine.
		If == 0, it cannot be added to the inventory.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", EditCondition = "CanStack"), Category = "InventoryItemInfo")
		int32 MaxInStack = 1;
};
