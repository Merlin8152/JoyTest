// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Inventory/DragDrop/JT_DragDropStructs.h"
#include "JT_InventoryBaseSlotWD.generated.h"


class UJT_InventoryItemInfo;



class UJT_InventoryComponent;
class UJT_InventoryWD;
class AJT_GameHUD;
class UJT_InventoryItemInfo;
class UJT_DragDropSlotWD;

class UDragDropOperation;
class UTextBlock;
class UImage;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_InventoryBaseSlotWD : public UUserWidget
{
	GENERATED_BODY()

		//Constructor
public:

	UJT_InventoryBaseSlotWD(const FObjectInitializer& ObjectInitializer);



	//C++ public methods
public:


	virtual bool SetNewItemInfo(UJT_InventoryItemInfo* NewItemInfo, bool bUpdateSlot);



	//C++ protected methods
protected:

	virtual void NativeDestruct() override;



	//..................................................................Native Inputs.............................................................................//

	/*
		Starts drag drop tracking inside
	*/
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//................................................................................................................................//



	/*
		Performs creating drag drop operation
	*/
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;


	//c++ protected values
protected:

	FVector2D ViewportPosition;

	//c++ private values
private:


	//>>>...............................................................................................................................................................................<<<//
	//>>>...............................................................................................................................................................................<<<//




	//Blueprint public methods
public:

	//..................................................................Initializes.............................................................................//

	UFUNCTION(BlueprintCallable, Category = "InventoryBaseSlotWD")
		void InitializesBaseSlot(UJT_InventoryWD* NewInventoryWD, UJT_InventoryComponent* NewInventoryComponent, AJT_GameHUD* NewGameHUD);

	//................................................................................................................................//





	//..................................................................DragDrop..........................................................................//

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventoryBaseSlotWD")
		bool HandleDragDrop(UDragDropOperation* Operation, bool bUpdateSlot);
	virtual bool HandleDragDrop_Implementation(UDragDropOperation* Operation, bool bUpdateSlot);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventoryBaseSlotWD")
		bool ApplyDragDropData(const FInventoryDragDropData& InData);
	virtual bool ApplyDragDropData_Implementation(const FInventoryDragDropData& InData);


	/** Creates drag drop data with assigned item info and widget */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventoryBaseSlotWD")
		virtual void CreateDragDropData(FInventoryDragDropData& OutData);

	/** Creates drag drop operation object */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventoryBaseSlotWD")
		UDragDropOperation* CreateDragDropOperation();

	//................................................................................................................................//






	//..................................................................Item Info........................................................//


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventoryBaseSlotWD")
		void ClearSlotInfo();
	void ClearSlotInfo_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventoryBaseSlotWD")
		void UpdateSlotInfo();
	void UpdateSlotInfo_Implementation();


	/** Checks if the slot is free */
	UFUNCTION(BlueprintCallable, Category = "InventoryBaseSlotWD")
		bool IsSlotFree() const;


	//................................................................................................................................//



//Blueprint variables
public:


	/*
		Opacity of Slot Icon Image.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlotWD")
		float SlotIconOpacity = 0.4f;

	/*
		Holds current information about item
	*/
	UPROPERTY(BlueprintReadOnly, Category = "InventoryBaseSlotWD | Widgets", meta = (ExposeOnSpawn = "true"))
		UJT_InventoryItemInfo* ItemInfo = nullptr;
	/*
		Text block for Slot Items count.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "InventoryBaseSlotWD | Widgets")
		UTextBlock* ItemCount = nullptr;
	/*
		Image for Slot Icon.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "InventoryBaseSlotWD | Widgets")
		UImage* ItemIcon = nullptr;


	/*
		Drag drop default visual class
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryBaseSlotWD | DragnDrop")
		TSubclassOf<UJT_DragDropSlotWD> DragDropVisualClass;




	/*
		InventoryComponent of owning Character
	*/
	UPROPERTY(BlueprintReadOnly)
		UJT_InventoryComponent* InventoryComponent = nullptr;


	UPROPERTY(BlueprintReadOnly)
		AJT_GameHUD* GameHUD = nullptr;

	UPROPERTY(BlueprintReadOnly)
		UJT_InventoryWD* InventoryWD = nullptr;
};
