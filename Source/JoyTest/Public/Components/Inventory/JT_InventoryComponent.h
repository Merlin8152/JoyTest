// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JT_InventoryComponent.generated.h"


class UJT_InventoryItemInfo;
class UJT_InventorySlotBase;



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDropped, AActor*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotChanged, int, QuickSlotIndex);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYTEST_API UJT_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJT_InventoryComponent();

	//c++ public methods
public:

	//UActorComponent
	virtual void BeginPlay() override;
	void CreateInventorySlots();
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~UActorComponent

	//c++ protected methods
protected:

	FTransform GetDroppingItemTransform() const;





	//c++ public values
public:

	AActor* MyActor = nullptr;


	//c++ private values
private:




	//>>..................................................................................................................................................................<<//



	//Blueprint methods
public:

	//..........................................Inventory.........................................................//
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
		bool GetCanCollectItem(UJT_InventoryItemInfo* ItemInfo) const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
		void CollectItem(UJT_InventoryItemInfo* ItemInfo);


	UPROPERTY(BlueprintAssignable, Category = "InventorySystem|EventsForBind")
		FOnInventoryChanged OnInventoryChangedBind;

	UPROPERTY(BlueprintAssignable, Category = "InventorySystem|EventsForBind")
		FOnItemDropped OnItemDroppedBind;



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void GetItemsWhichCouldBeAddedToInventory(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot, TArray<UJT_InventoryItemInfo*>& OutItemsInfo) const;
	virtual void GetItemsWhichCouldBeAddedToInventory_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot, TArray<UJT_InventoryItemInfo*>& OutItemsInfo) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		int GetItemsCountWhichCouldBeAddedToInventory(UJT_InventoryItemInfo* SimilarItemInfo, bool MustBeNewSlot) const;
	virtual int GetItemsCountWhichCouldBeAddedToInventory_Implementation(UJT_InventoryItemInfo* SimilarItemInfo, bool MustBeNewSlot) const;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		bool GetCanAddItemToInventory(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt, int& PotentialSlotIndex) const;
	virtual bool GetCanAddItemToInventory_Implementation(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt, int& PotentialSlotIndex) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		int GetItemPotentialInventorySlotIndex(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt) const;
	virtual int GetItemPotentialInventorySlotIndex_Implementation(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt) const;




	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void AddItemsToInventory(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot);
	virtual void AddItemsToInventory_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void AddSimilarItemsToInventory(UJT_InventoryItemInfo* SimilarItemInfo, int Count, bool MustBeNewSlot);
	virtual void AddSimilarItemsToInventory_Implementation(UJT_InventoryItemInfo* SimilarItemInfo, int Count, bool MustBeNewSlot);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void AddItemsFromActorsToInventory(const TArray<TSubclassOf<AActor>>& ActorClasses, bool MustBeNewSlot);
	virtual void AddItemsFromActorsToInventory_Implementation(const TArray<TSubclassOf<AActor>>& ActorClasses, bool MustBeNewSlot);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void AddItemsFromSoftActorsToInventory(const TArray<TSoftClassPtr<AActor>>& ActorClasses, bool MustBeNewSlot);
	virtual void AddItemsFromSoftActorsToInventory_Implementation(const TArray<TSoftClassPtr<AActor>>& ActorClasses, bool MustBeNewSlot);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void RemoveFromInventory(int32 SlotIndex, int32 Count);
	virtual void RemoveFromInventory_Implementation(int32 SlotIndex, int32 Count);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void DropItemFromInventory(int32 SlotIndex, int32 Count, bool SimulatePhysics);
	virtual void DropItemFromInventory_Implementation(int32 SlotIndex, int32 Count, bool SimulatePhysics);
	/*
		Call it after removing items from slot. NOT CHECK INDEX.
	*/
	UFUNCTION(BlueprintCallable, Category = "InventorySystem|Inventory")
		void ApplySlotDeleteChanges(int SlotIndex);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void MoveItemsCountFromSlotToSlot(int FromSlotIndex, int ToSlotIndex, int Count);
	virtual void MoveItemsCountFromSlotToSlot_Implementation(int FromSlotIndex, int ToSlotIndex, int Count);



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void StackAllInInventory(bool MoveAllToStart);
	void StackAllInInventory_Implementation(bool MoveAllToStart);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void SortInventoryByItemNames(bool Reverse);
	void SortInventoryByItemNames_Implementation(bool Reverse);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void SortInventoryByEmptySlots(bool Reverse);
	void SortInventoryByEmptySlots_Implementation(bool Reverse);



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void ClampInventoryToMaxSize(bool DropExcludedItems, bool SimulatePhysics);
	void ClampInventoryToMaxSize_Implementation(bool DropExcludedItems, bool SimulatePhysics);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void ClearAllEmptySlots();
	void ClearAllEmptySlots_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void ClearEmptySlot(int SlotIndex);
	void ClearEmptySlot_Implementation(int SlotIndex);




	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		void FindInventoryItemSlotsByName(const FName& ItemName, TArray<int32>& OutSlots) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		void FindInventoryItemSlotsByClass(UClass* ItemClass, TArray<int32>& OutSlots) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		int32 FindFirstEmptySlot(int32 StartAt) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		int32 FindFirstSimilarNotFullSlot(int32 StartAt, UJT_InventoryItemInfo* SimilarToItemInfo) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void FindAndDropItemsFromInventoryByName(const FName& ItemName, int32 Count, bool SimulatePhysics);
	void FindAndDropItemsFromInventoryByName_Implementation(const FName& ItemName, int32 Count, bool SimulatePhysics);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void FindAndDropItemsFromInventoryByClass(UClass* ItemClass, int32 Count, bool SimulatePhysics);
	void FindAndDropItemsFromInventoryByClass_Implementation(UClass* ItemClass, int32 Count, bool SimulatePhysics);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		int GetEmptySlotsCount(int StartAt = 0) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "InventorySystem|Inventory")
		int GetNonEmptySlotsCount(int StartAt = 0) const;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|Inventory")
		void CleanInventory();
	virtual void CleanInventory_Implementation();

	//............................................................................................................//


	//.................................QuickSlots.................................................................//

	UPROPERTY(BlueprintAssignable, Category = "InventorySystem|EventsForBind")
		FOnQuickSlotChanged OnQuickSlotChangedBind;


	UFUNCTION(BlueprintCallable, Category = "InventorySystem|QuickSlots")
		void RegisterQuickSlots(int Num); //only on server


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void ClearQuickSlot(int QuickSlotIndex);
	virtual void ClearQuickSlot_Implementation(int QuickSlotIndex);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void RemoveItemCountFromQuickSlot(int QuickSlotIndex, int Count, bool ClearIfEmpty);
	virtual void RemoveItemCountFromQuickSlot_Implementation(int QuickSlotIndex, int Count, bool ClearIfEmpty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void DropItemFromQuickSlot(int QuickSlotIndex, int32 Count, bool SimulatePhysics, bool ClearIfEmpty);
	virtual void DropItemFromQuickSlot_Implementation(int QuickSlotIndex, int32 Count, bool SimulatePhysics, bool ClearIfEmpty);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void SetQuickSlotByInventorySlot(int QuickSlotIndex, int InventorySlotIndex, bool CacheSlots);
	virtual void SetQuickSlotByInventorySlot_Implementation(int QuickSlotIndex, int InventorySlotIndex, bool CacheSlots);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void SetQuickSlotBySlotInfo(int QuickSlotIndex, UJT_InventorySlotBase* SlotInfo, bool CacheSlots);
	virtual void SetQuickSlotBySlotInfo_Implementation(int QuickSlotIndex, UJT_InventorySlotBase* SlotInfo, bool CacheSlots);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void SetQuickSlotByItemInfo(int QuickSlotIndex, UJT_InventoryItemInfo* ItemInfo);
	virtual void SetQuickSlotByItemInfo_Implementation(int QuickSlotIndex, UJT_InventoryItemInfo* ItemInfo);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure = false, Category = "InventorySystem|Inventory")
		bool GetCanAddItemToAnyQuickSlot(UJT_InventoryItemInfo* ItemInfo, bool ShouldBeNotEmpty, int& PotentialQuickSlotIndex) const;
	virtual bool GetCanAddItemToAnyQuickSlot_Implementation(UJT_InventoryItemInfo* ItemInfo, bool ShouldBeNotEmpty, int& PotentialQuickSlotIndex) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void AddSimilarItemsToQuickSlots(UJT_InventoryItemInfo* ItemInfo, int Count, bool ShouldBeNotEmpty);
	virtual void AddSimilarItemsToQuickSlots_Implementation(UJT_InventoryItemInfo* ItemInfo, int Count, bool ShouldBeNotEmpty);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void AddItemsToQuickSlots(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool ShouldBeNotEmpty);
	virtual void AddItemsToQuickSlots_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool ShouldBeNotEmpty);




	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void MoveItemsCountFromQuickSlotToQuickSlot(int FromQuickSlotIndex, int ToQuickSlotIndex, int Count, bool ClearIfEmpty);
	virtual void MoveItemsCountFromQuickSlotToQuickSlot_Implementation(int FromQuickSlotIndex, int ToQuickSlotIndex, int Count, bool ClearIfEmpty);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void MoveItemsFromQuickSlotToInventory(int QuickSlotIndex, int Count, bool MustBeNewSlot, bool ClearIfEmpty);
	virtual void MoveItemsFromQuickSlotToInventory_Implementation(int QuickSlotIndex, int Count, bool MustBeNewSlot, bool ClearIfEmpty);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void MoveAllItemsFromQuickSlotsToInventory(bool MustBeNewSlot, bool ClearIfEmpty);
	virtual void MoveAllItemsFromQuickSlotsToInventory_Implementation(bool MustBeNewSlot, bool ClearIfEmpty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void MoveItemsFromInventoryToQuickSlot(int InventorySlotIndex, int QuickSlotIndex, int Count, bool ApplyDeleteChanges = true);
	virtual void MoveItemsFromInventoryToQuickSlot_Implementation(int InventorySlotIndex, int QuickSlotIndex, int Count, bool ApplyDeleteChanges = true);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventorySystem|QuickSlots")
		void MoveItemsFromInventoryToAnyQuickSlot(int InventorySlotIndex, int Count, bool ShouldBeNotEmpty, bool ApplyDeleteChanges = true);
	virtual void MoveItemsFromInventoryToAnyQuickSlot_Implementation(int InventorySlotIndex, int Count, bool ShouldBeNotEmpty, bool ApplyDeleteChanges = true);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem|QuickSlots")
		bool GetIsQuickSlotEmpty(int QuickSlotIndex) const;

	//............................................................................................................//




	//Blueprint values
public:

	//..........................................Inventory.........................................................//

	/*
		Array of inventory slots.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		TArray<UJT_InventorySlotBase*> Inventory;
	/*
		Max available slots in Inventory.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"), Category = "Inventory")
		int32 MaxInventorySlotsCount = 10;


	/*
		Forward distance offset of spawning dropped items.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Inventory")
		float DropFromInventoryDistance = 100.0f;
	/*
		Right distance offset of spawning dropped items.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Inventory")
		float DropFromInventoryRightOffset = 0.0f;

	//............................................................................................................//



	//.................................QuickSlots.................................................................//

	/*
		Initial QuickSlots count.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"), Category = "QuickSlots")
		int SpawnQuickSlotCount = 5;
	/*
		Additional slots.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "QuickSlots")
		TArray<UJT_InventorySlotBase*> QuickSlots;
	//............................................................................................................//

};
