// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JT_InventorySlotBase.generated.h"




class UJT_InventoryItemInfo;
class UJT_InventoryComponent;



/*
	 Storage for items.
	 Use OnInventoryChanged and ClearEmptySlots on parent InventoryComponent where it need.

	 @see UPZ_InventoryItemInfo.
*/
UCLASS(Blueprintable, BlueprintType)
class JOYTEST_API UJT_InventorySlotBase : public UObject
{
	GENERATED_BODY()

		//constructor
public:

	UJT_InventorySlotBase();


	

	//c++ public methods
public:


	virtual UWorld* GetWorld() const override;

	virtual void PostInitProperties() override;




//c++ protected values
protected:

	/*
		Inventory component owner.
	*/
	UJT_InventoryComponent* MyInventoryComponent = nullptr;





	//>>...............................................................................................................................<<//



	//Blueprint methods
public:

	/*
		@return true if this slot not contains any items.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		bool IsEmpty() const;
	virtual bool IsEmpty_Implementation() const;
	/*
		@return true if this slot contains maximal items count
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		bool IsFull() const;
	virtual bool IsFull_Implementation() const;
	/*
		@param IfAddCount - Count of potential objects to add
		@param DefaultMaxInSlot - Max items count for this slot if now it not cantains any items
		@return true if this slot will full if add some items count.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		bool WillFull(int IfAddCount, int DefaultMaxInSlot) const;
	virtual bool WillFull_Implementation(int IfAddCount, int DefaultMaxInSlot) const;




	/*
		@return Count of items in this slot.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		int GetItemsCount() const;
	virtual int GetItemsCount_Implementation() const;

	/*
		@return First item from this slot as an image of this slot. It will be used as information of items in this slot.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		UJT_InventoryItemInfo* GetFirstItem() const;
	virtual UJT_InventoryItemInfo* GetFirstItem_Implementation() const;

	/*
		@return true if this slot is equal to other slot.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		bool IsEqual(UJT_InventorySlotBase* OtherSlot) const;
	virtual bool IsEqual_Implementation(UJT_InventorySlotBase* OtherSlot) const;




	/*
		@return true if ItemInfo can be added to this slot.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "InventorySlotBase")
		bool GetCanAddItem(UJT_InventoryItemInfo* ItemInfo);
	virtual bool GetCanAddItem_Implementation(UJT_InventoryItemInfo* ItemInfo);
	/*
		@return Success.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		bool AddItem(UJT_InventoryItemInfo* ItemInfo); //only on server
	virtual bool AddItem_Implementation(UJT_InventoryItemInfo* ItemInfo);


	/*
		Remove items from the end from this slot.
		@param Count - Count of removing items
		@return Removed items count.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		int RemoveLastItemsCount(int Count); //only on server
	virtual int RemoveLastItemsCount_Implementation(int Count);
	/*
		Remove one last item from this slot.
		@return Success.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		bool RemoveLastItem(); //only on server
	virtual bool RemoveLastItem_Implementation();

	/*
		@param Count - Count of dropping items
		@param SimulatePhysics - Enable simulate physics for all dropped items
		@param SpawnTransform - Initial spawn transform for dropping objects
		@return Dropped items count.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		int DropLastItemsCount(int Count, bool SimulatePhysics, const FTransform& SpawnTransform); //only on server
	virtual int DropLastItemsCount_Implementation(int Count, bool SimulatePhysics, const FTransform& SpawnTransform);

	/*
		Remove all information about items and set slot info to default.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		void Clear(); //only on server
	virtual void Clear_Implementation();




	/*
		Add items from this slot to other slot.
		After moving items remove their from this slot.
		@param Count - Count of moving items
		@return Success.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		bool MoveLastItemsCountToOtherSlot(int Count, UJT_InventorySlotBase* OtherSlot); //only on server
	virtual bool MoveLastItemsCountToOtherSlot_Implementation(int Count, UJT_InventorySlotBase* OtherSlot);

	/*
	Add items from this slot to inventory.
	After moving items remove their from this slot.
	@param Count - Count of moving items
	@param MustBeNewSlot - if true then inventory should create new slot for moving items
	@return Success.
*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySlotBase")
		bool MoveLastItemsCountToInventory(int Count, bool MustBeNewSlot, UJT_InventoryComponent* OtherInventory); //only on server
	virtual bool MoveLastItemsCountToInventory_Implementation(int Count, bool MustBeNewSlot, UJT_InventoryComponent* OtherInventory);


	//Blueprint values
public:
	/*
		Array of items in this slot
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryUniqueSlot")
		TArray<UJT_InventoryItemInfo*> ItemsInfo;
};