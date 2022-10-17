// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/JT_InventoryComponent.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"
#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"

#include "Components/Inventory/JT_InventoryFunctionLibrary.h"

UJT_InventoryComponent::UJT_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MyActor = GetOwner();
}



void UJT_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateInventorySlots();
	RegisterQuickSlots(SpawnQuickSlotCount);
}

void UJT_InventoryComponent::CreateInventorySlots()
{
	for (int i = 0; i < MaxInventorySlotsCount; ++i)
	{
		UJT_InventorySlotBase* LBaseSlot = NewObject<UJT_InventorySlotBase>(this);
		Inventory.Add(LBaseSlot);
	}
}

void UJT_InventoryComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	//TODO on child
}

void UJT_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Not available on this component
}






//...............................................Inventory............................................................................//

bool UJT_InventoryComponent::GetCanCollectItem(UJT_InventoryItemInfo* ItemInfo) const
{
	int PotentialSlotIndex;
	TArray<UJT_InventoryItemInfo*> ItemsToAdd;
	ItemsToAdd.Add(ItemInfo);

	return /*GetCanAddItemToAnyQuickSlot(ItemInfo, true, PotentialSlotIndex) || */GetCanAddItemToInventory(ItemInfo, false, 0, PotentialSlotIndex);

}

void UJT_InventoryComponent::CollectItem(UJT_InventoryItemInfo* ItemInfo)
{
	int PotentialSlotIndex;
	TArray<UJT_InventoryItemInfo*> ItemsToAdd;
	ItemsToAdd.Add(ItemInfo);

	if (GetCanAddItemToInventory(ItemInfo, false, 0, PotentialSlotIndex))
	{
		AddItemsToInventory(ItemsToAdd, false);
	}
}

void UJT_InventoryComponent::GetItemsWhichCouldBeAddedToInventory_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot, TArray<UJT_InventoryItemInfo*>& OutItemsInfo) const
{
	OutItemsInfo.Empty();


	if (MustBeNewSlot)
	{
		int EmptySlotsCount = GetEmptySlotsCount(0);
		int ItemsToAddCount = FMath::Min<int>(EmptySlotsCount, ItemsInfo.Num());

		for (int i = 0; i < ItemsToAddCount; ++i)
		{
			OutItemsInfo.Add(ItemsInfo[i]);
		}

		return;
	}



	for (UJT_InventoryItemInfo* LItemInfo : ItemsInfo)
	{
		int PotentialSlotIndex = -1;
		if (GetCanAddItemToInventory(LItemInfo, false, 0, PotentialSlotIndex))
		{
			OutItemsInfo.Add(LItemInfo);
		}
	}
}

int UJT_InventoryComponent::GetItemsCountWhichCouldBeAddedToInventory_Implementation(UJT_InventoryItemInfo* SimilarItemInfo, bool MustBeNewSlot) const
{
	if (!IsValid(SimilarItemInfo)) return 0;


	if (MustBeNewSlot || !SimilarItemInfo->CanStack) return GetEmptySlotsCount(0);


	int LItemsCount = 0;
	for (UJT_InventorySlotBase* LSlot : Inventory)
	{
		if (LSlot->IsEmpty())
		{
			LItemsCount += SimilarItemInfo->MaxInStack;
			continue;
		}

		if (!LSlot->IsFull() && UJT_InventoryFunctionLibrary::AreTwoItemsInfoIdentical(LSlot->GetFirstItem(), SimilarItemInfo))
		{
			LItemsCount += SimilarItemInfo->MaxInStack - LSlot->GetItemsCount();
			continue;
		}
	}

	return LItemsCount;
}



bool UJT_InventoryComponent::GetCanAddItemToInventory_Implementation(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt, int& PotentialSlotIndex) const
{
	// We do not check for the presence of a component, since it is still possible to add an item without a component, but with limited functionality
	if (!IsValid(ItemInfo) || !ItemInfo->IsInfoDataValid())
	{
		PotentialSlotIndex = -1;
		return false;
	}


	PotentialSlotIndex = GetItemPotentialInventorySlotIndex(ItemInfo, MustBeNewSlot, StartAt);

	return PotentialSlotIndex >= 0;
}

int UJT_InventoryComponent::GetItemPotentialInventorySlotIndex_Implementation(UJT_InventoryItemInfo* ItemInfo, bool MustBeNewSlot, int StartAt) const
{
	if (!IsValid(ItemInfo) || StartAt < 0 || StartAt >= MaxInventorySlotsCount) return -1;


	if (!ItemInfo->CanStack || MustBeNewSlot)
	{
		return FindFirstEmptySlot(StartAt);
	}

	int PotentialSlotIndex = FindFirstSimilarNotFullSlot(StartAt, ItemInfo);
	if (PotentialSlotIndex < 0)
	{
		return FindFirstEmptySlot(StartAt);
	}

	return PotentialSlotIndex;
}






void UJT_InventoryComponent::AddItemsToInventory_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool MustBeNewSlot)
{
	bool SuccessAdded = false;
	for (UJT_InventoryItemInfo* LItemInfo : ItemsInfo)
	{
		int PotentialIndex = -1;
		if (GetCanAddItemToInventory(LItemInfo, MustBeNewSlot, 0, PotentialIndex))
		{
			if (!Inventory.IsValidIndex(PotentialIndex))
			{
				PotentialIndex = Inventory.Add(NewObject<UJT_InventorySlotBase>(this, LItemInfo->InventorySlotClass));
			}
			else if (Inventory[PotentialIndex]->IsEmpty())
			{
				Inventory[PotentialIndex] = NewObject<UJT_InventorySlotBase>(this, LItemInfo->InventorySlotClass);
			}

			Inventory[PotentialIndex]->AddItem(LItemInfo);

			SuccessAdded = true;
		}
	}

	if (SuccessAdded)
	{
		OnInventoryChangedBind.Broadcast();
	}
}

void UJT_InventoryComponent::AddSimilarItemsToInventory_Implementation(UJT_InventoryItemInfo* SimilarItemInfo, int Count, bool MustBeNewSlot)
{
	int PotentialIndex = -1;
	if (Count <= 0 || !GetCanAddItemToInventory(SimilarItemInfo, MustBeNewSlot, 0, PotentialIndex)) return;


	while (PotentialIndex >= 0 && Count > 0)
	{
		if (!Inventory.IsValidIndex(PotentialIndex))
		{
			PotentialIndex = Inventory.Add(NewObject<UJT_InventorySlotBase>(this, SimilarItemInfo->InventorySlotClass));
		}
		else
			if (Inventory[PotentialIndex]->IsEmpty())
			{
				Inventory[PotentialIndex] = NewObject<UJT_InventorySlotBase>(this, SimilarItemInfo->InventorySlotClass);
			}

		Inventory[PotentialIndex]->AddItem(DuplicateObject<UJT_InventoryItemInfo>(SimilarItemInfo, nullptr));


		--Count;
		PotentialIndex = GetItemPotentialInventorySlotIndex(SimilarItemInfo, MustBeNewSlot, PotentialIndex);
	}

	OnInventoryChangedBind.Broadcast();
}






void UJT_InventoryComponent::RemoveFromInventory_Implementation(int32 SlotIndex, int32 Count)
{
	if (!Inventory.IsValidIndex(SlotIndex) || Count <= 0 || Inventory[SlotIndex]->IsEmpty()) return;


	Inventory[SlotIndex]->RemoveLastItemsCount(Count);

	ApplySlotDeleteChanges(SlotIndex);
}

void UJT_InventoryComponent::DropItemFromInventory_Implementation(int32 SlotIndex, int32 Count, bool SimulatePhysics)
{
	if (!Inventory.IsValidIndex(SlotIndex) || Inventory[SlotIndex]->IsEmpty() || Count <= 0) return;


	Inventory[SlotIndex]->DropLastItemsCount(Count, SimulatePhysics, GetDroppingItemTransform());

	ApplySlotDeleteChanges(SlotIndex);
}

void UJT_InventoryComponent::ApplySlotDeleteChanges(int SlotIndex)
{
	if (Inventory[SlotIndex]->IsEmpty())
	{
		ClearEmptySlot(SlotIndex);
	}
	else
	{
		OnInventoryChangedBind.Broadcast();
	}
}







void UJT_InventoryComponent::MoveItemsCountFromSlotToSlot_Implementation(int FromSlotIndex, int ToSlotIndex, int Count)
{
	if (!Inventory.IsValidIndex(FromSlotIndex) || !Inventory.IsValidIndex(ToSlotIndex) || Count <= 0) return;


	if (Inventory[FromSlotIndex]->MoveLastItemsCountToOtherSlot(Count, Inventory[ToSlotIndex]))
	{
		ApplySlotDeleteChanges(FromSlotIndex);
	}
}





void UJT_InventoryComponent::StackAllInInventory_Implementation(bool MoveAllToStart)
{
	for (int i = 0; i < Inventory.Num() - 1; ++i)
	{
		if (Inventory[i]->IsEmpty() || Inventory[i]->IsFull() || !Inventory[i]->GetFirstItem()->CanStack) continue;


		for (int j = i + 1; j < Inventory.Num(); ++j)
		{
			if (Inventory[j]->IsEmpty() || Inventory[j]->IsFull()) continue;


			if (UJT_InventoryFunctionLibrary::AreTwoSlotsEqual(Inventory[i], Inventory[j]))
			{
				Inventory[j]->MoveLastItemsCountToOtherSlot(Inventory[j]->GetItemsCount(), Inventory[i]);
			}
		}
	}

	ClearAllEmptySlots();

	if (MoveAllToStart)
	{
		SortInventoryByEmptySlots_Implementation(false);
	}
}


void UJT_InventoryComponent::SortInventoryByItemNames_Implementation(bool Reverse)
{
	Inventory.Sort(
		[Reverse](const UJT_InventorySlotBase& One, const UJT_InventorySlotBase& Two)
		{
			if (One.IsEmpty() && Two.IsEmpty())
			{
				return true;
			}
			if (One.IsEmpty())
			{
				return false;
			}
			if (Two.IsEmpty())
			{
				return true;
			}

			return (Reverse) ? Two.GetFirstItem()->ItemName.LexicalLess(One.GetFirstItem()->ItemName) : One.GetFirstItem()->ItemName.LexicalLess(Two.GetFirstItem()->ItemName);
		});

	OnInventoryChangedBind.Broadcast();
}



void UJT_InventoryComponent::SortInventoryByEmptySlots_Implementation(bool Reverse)
{
	Inventory.Sort(
		[Reverse](const UJT_InventorySlotBase& One, const UJT_InventorySlotBase& Two)
		{
			if (One.IsEmpty() == Two.IsEmpty())
			{
				return true;
			}

			return (Reverse) ? One.IsEmpty() && !Two.IsEmpty() : !One.IsEmpty() && Two.IsEmpty();
		});

	OnInventoryChangedBind.Broadcast();
}





void UJT_InventoryComponent::ClampInventoryToMaxSize_Implementation(bool DropExcludedItems, bool SimulatePhysics)
{
	int LInventorySize = Inventory.Num();


	for (int i = LInventorySize - 1; i > MaxInventorySlotsCount; --i)
	{
		if (DropExcludedItems)
		{
			DropItemFromInventory(i, Inventory[i]->GetItemsCount(), SimulatePhysics);
		}
		else
		{
			RemoveFromInventory(i, Inventory[i]->GetItemsCount());
		}

		Inventory[i]->Rename(nullptr, nullptr);
		Inventory.RemoveAt(i);
	}



	for (int i = 0; i < MaxInventorySlotsCount - LInventorySize; ++i)
	{
		UJT_InventorySlotBase* LBaseSlot = NewObject<UJT_InventorySlotBase>(this);
		Inventory.Add(LBaseSlot);
	}


	OnInventoryChangedBind.Broadcast();
}


void UJT_InventoryComponent::ClearAllEmptySlots_Implementation()
{
	bool WasCleared = false;
	for (int i = Inventory.Num() - 1; i >= 0; --i)
	{
		if (Inventory[i]->IsEmpty())
		{	
			Inventory[i]->Clear();

			WasCleared = true;
		}
	}


	if (WasCleared) OnInventoryChangedBind.Broadcast();
}

void UJT_InventoryComponent::ClearEmptySlot_Implementation(int SlotIndex)
{
	if (!Inventory.IsValidIndex(SlotIndex) || !Inventory[SlotIndex]->IsEmpty()) return;


	Inventory[SlotIndex]->Clear();


	OnInventoryChangedBind.Broadcast();
}





void UJT_InventoryComponent::FindInventoryItemSlotsByName(const FName& ItemName, TArray<int32>& OutSlots) const
{
	OutSlots.Empty();
	if (ItemName.IsNone()) return;


	for (int i = 0; i < Inventory.Num(); ++i)
	{
		if (!Inventory[i]->IsEmpty() && Inventory[i]->GetFirstItem()->ItemName == ItemName)
		{
			OutSlots.Add(i);
		}
	}
}

void UJT_InventoryComponent::FindInventoryItemSlotsByClass(UClass* ItemClass, TArray<int32>& OutSlots) const
{
	OutSlots.Empty();
	if (!IsValid(ItemClass)) return;


	for (int i = 0; i < Inventory.Num(); ++i)
	{
		if (!Inventory[i]->IsEmpty() && Inventory[i]->GetFirstItem()->ItemClass->IsChildOf(ItemClass))
		{
			OutSlots.Add(i);
		}
	}
}



int32 UJT_InventoryComponent::FindFirstEmptySlot(int32 StartAt) const
{
	if (StartAt < 0) return -1;

	for (int32 i = StartAt; i < Inventory.Num(); ++i)
	{
		if (Inventory[i]->IsEmpty()) return i;
	}

	return -1;
}

int32 UJT_InventoryComponent::FindFirstSimilarNotFullSlot(int32 StartAt, UJT_InventoryItemInfo* SimilarToItemInfo) const
{
	if (StartAt < 0 || !IsValid(SimilarToItemInfo)) return -1;

	for (int32 i = StartAt; i < Inventory.Num(); ++i)
	{
		if (
			!Inventory[i]->IsEmpty() && !Inventory[i]->IsFull() &&
			UJT_InventoryFunctionLibrary::AreTwoItemsInfoIdentical(Inventory[i]->GetFirstItem(), SimilarToItemInfo)
			)
		{
			return i;
		}
	}

	return -1;
}



void UJT_InventoryComponent::FindAndDropItemsFromInventoryByName_Implementation(const FName& ItemName, int32 Count, bool SimulatePhysics)
{
	if (Count <= 0 || ItemName.IsNone()) return;


	for (int i = 0; i < Inventory.Num(); ++i)
	{
		if (!Inventory[i]->IsEmpty() && Inventory[i]->GetFirstItem()->ItemName == ItemName)
		{
			int CountToDrop = FMath::Min<int>(Count, Inventory[i]->GetItemsCount());
			Count -= CountToDrop;


			Inventory[i]->DropLastItemsCount(CountToDrop, SimulatePhysics, GetDroppingItemTransform());

			if (Count == 0) break;
		}
	}

	ClearAllEmptySlots_Implementation();
	OnInventoryChangedBind.Broadcast();
}

void UJT_InventoryComponent::FindAndDropItemsFromInventoryByClass_Implementation(UClass* ItemClass, int32 Count, bool SimulatePhysics)
{
	if (Count <= 0 || !IsValid(ItemClass)) return;


	for (int i = 0; i < Inventory.Num(); ++i)
	{
		if (!Inventory[i]->IsEmpty() && Inventory[i]->GetFirstItem()->ItemClass->IsChildOf(ItemClass))
		{
			int CountToDrop = FMath::Min<int>(Count, Inventory[i]->GetItemsCount());
			Count -= CountToDrop;



			Inventory[i]->DropLastItemsCount(CountToDrop, SimulatePhysics, GetDroppingItemTransform());

			if (Count == 0) break;
		}
	}

	ClearAllEmptySlots_Implementation();
	OnInventoryChangedBind.Broadcast();
}



int UJT_InventoryComponent::GetEmptySlotsCount(int StartAt) const
{
	int LEmptySlotsCount = 0;
	for (int i = StartAt; i < Inventory.Num(); ++i)
	{
		if (Inventory[i]->IsEmpty()) ++LEmptySlotsCount;
	}

	return LEmptySlotsCount;
}

int UJT_InventoryComponent::GetNonEmptySlotsCount(int StartAt) const
{
	int LNonEmptySlotsCount = 0;

	for (int i = StartAt; i < Inventory.Num(); ++i)
	{
		if (!Inventory[i]->IsEmpty()) ++LNonEmptySlotsCount;
	}

	return LNonEmptySlotsCount;
}


void UJT_InventoryComponent::CleanInventory_Implementation()
{
	for (int i = 0; i < MaxInventorySlotsCount; ++i)
	{
		Inventory[i]->Clear();
	}
}

void UJT_InventoryComponent::RegisterQuickSlots(int Num)
{
	QuickSlots.Empty();

	while (Num > 0)
	{
		--Num;
		UJT_InventorySlotBase* NewQuickSlot = NewObject<UJT_InventorySlotBase>(this);
		QuickSlots.Add(NewQuickSlot);
	}
}

void UJT_InventoryComponent::ClearQuickSlot_Implementation(int QuickSlotIndex)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex)) return;

	QuickSlots[QuickSlotIndex]->Clear();
	OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
}

void UJT_InventoryComponent::RemoveItemCountFromQuickSlot_Implementation(int QuickSlotIndex, int Count, bool ClearIfEmpty)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || Count <= 0) return;


	QuickSlots[QuickSlotIndex]->RemoveLastItemsCount(Count);

	if (QuickSlots[QuickSlotIndex]->IsEmpty() && ClearIfEmpty)
	{
		QuickSlots[QuickSlotIndex]->Clear();
	}

	OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
}

void UJT_InventoryComponent::DropItemFromQuickSlot_Implementation(int QuickSlotIndex, int32 Count, bool SimulatePhysics, bool ClearIfEmpty)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || Count <= 0) return;


	QuickSlots[QuickSlotIndex]->DropLastItemsCount(Count, SimulatePhysics, GetDroppingItemTransform());

	if (QuickSlots[QuickSlotIndex]->IsEmpty() && ClearIfEmpty)
	{
		QuickSlots[QuickSlotIndex]->Clear();
	}

	OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
}

void UJT_InventoryComponent::SetQuickSlotByInventorySlot_Implementation(int QuickSlotIndex, int InventorySlotIndex, bool CacheSlots)
{
	if (!Inventory.IsValidIndex(InventorySlotIndex)) return;

	SetQuickSlotBySlotInfo_Implementation(QuickSlotIndex, Inventory[InventorySlotIndex], CacheSlots);
}

void UJT_InventoryComponent::SetQuickSlotBySlotInfo_Implementation(int QuickSlotIndex, UJT_InventorySlotBase* SlotInfo, bool CacheSlots)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || !IsValid(SlotInfo)) return;

	QuickSlots[QuickSlotIndex] = DuplicateObject<UJT_InventorySlotBase>(SlotInfo, this);
	OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
}

void UJT_InventoryComponent::SetQuickSlotByItemInfo_Implementation(int QuickSlotIndex, UJT_InventoryItemInfo* ItemInfo)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || !IsValid(ItemInfo)) return;

	QuickSlots[QuickSlotIndex] = NewObject<UJT_InventorySlotBase>(this, ItemInfo->InventorySlotClass);
	QuickSlots[QuickSlotIndex]->AddItem(DuplicateObject<UJT_InventoryItemInfo>(ItemInfo, nullptr));

	OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
}

bool UJT_InventoryComponent::GetCanAddItemToAnyQuickSlot_Implementation(UJT_InventoryItemInfo* ItemInfo, bool ShouldBeNotEmpty, int& PotentialQuickSlotIndex) const
{
	PotentialQuickSlotIndex = -1;

	UJT_InventorySlotBase* LQuickSlot = nullptr;
	for (int i = 0; i < QuickSlots.Num(); ++i)
	{
		if (!IsValid(QuickSlots[i]) || QuickSlots[i]->GetClass() == UJT_InventorySlotBase::StaticClass() || QuickSlots[i]->IsEmpty())
		{
			if (ShouldBeNotEmpty)
			{
				continue;
			}
			else
			{
				LQuickSlot = NewObject<UJT_InventorySlotBase>(GetTransientPackage(), ItemInfo->InventorySlotClass);
			}
		}
		else
		{
			LQuickSlot = QuickSlots[i];
		}


		if (LQuickSlot->GetCanAddItem(ItemInfo))
		{
			PotentialQuickSlotIndex = i;
			return true;
		}
	}

	return false;
}

void UJT_InventoryComponent::AddSimilarItemsToQuickSlots_Implementation(UJT_InventoryItemInfo* ItemInfo, int Count, bool ShouldBeNotEmpty)
{
	if (Count <= 0 || !IsValid(ItemInfo)) return;


	for (int i = 0; i < QuickSlots.Num() && Count > 0; ++i)
	{
		if (!IsValid(QuickSlots[i]) || QuickSlots[i]->GetClass() == UJT_InventorySlotBase::StaticClass() || QuickSlots[i]->IsEmpty())
		{
			if (ShouldBeNotEmpty)
			{
				continue;
			}
			else
			{
				QuickSlots[i] = NewObject<UJT_InventorySlotBase>(this, ItemInfo->InventorySlotClass);
			}
		}


		bool LWasAdded = false;
		while (QuickSlots[i]->GetCanAddItem(ItemInfo) && Count > 0)
		{
			QuickSlots[i]->AddItem(DuplicateObject<UJT_InventoryItemInfo>(ItemInfo, this));
			--Count;
			LWasAdded = true;
		}

		if (LWasAdded)
		{
			OnQuickSlotChangedBind.Broadcast(i);
		}
	}
}

void UJT_InventoryComponent::AddItemsToQuickSlots_Implementation(const TArray<UJT_InventoryItemInfo*>& ItemsInfo, bool ShouldBeNotEmpty)
{
	int ItemsCount = ItemsInfo.Num();
	if (ItemsCount <= 0) return;


	int LItemInfoIndex = 0;
	for (int i = 0; i < QuickSlots.Num() && LItemInfoIndex < ItemsCount; ++i)
	{
		if (!IsValid(QuickSlots[i]) || QuickSlots[i]->GetClass() == UJT_InventorySlotBase::StaticClass() || QuickSlots[i]->IsEmpty())
		{
			if (ShouldBeNotEmpty)
			{
				continue;
			}
			else
			{
				QuickSlots[i] = NewObject<UJT_InventorySlotBase>(this, ItemsInfo[LItemInfoIndex]->InventorySlotClass);
			}
		}


		bool LWasAdded = false;
		while (LItemInfoIndex < ItemsCount && QuickSlots[i]->GetCanAddItem(ItemsInfo[LItemInfoIndex]))
		{
			QuickSlots[i]->AddItem(DuplicateObject<UJT_InventoryItemInfo>(ItemsInfo[LItemInfoIndex], this));
			++LItemInfoIndex;
			LWasAdded = true;
		}

		if (LWasAdded)
		{
			OnQuickSlotChangedBind.Broadcast(i);
		}
	}
}

void UJT_InventoryComponent::MoveItemsCountFromQuickSlotToQuickSlot_Implementation(int FromQuickSlotIndex, int ToQuickSlotIndex, int Count, bool ClearIfEmpty)
{
	if (
		!QuickSlots.IsValidIndex(FromQuickSlotIndex) || !QuickSlots.IsValidIndex(ToQuickSlotIndex) || !IsValid(QuickSlots[FromQuickSlotIndex]) ||
		QuickSlots[FromQuickSlotIndex]->IsEmpty()
		) return;


	if (!IsValid(QuickSlots[ToQuickSlotIndex]) || QuickSlots[ToQuickSlotIndex]->GetClass() == UJT_InventorySlotBase::StaticClass())
	{
		QuickSlots[ToQuickSlotIndex] = NewObject<UJT_InventorySlotBase>(this, QuickSlots[FromQuickSlotIndex]->GetFirstItem()->InventorySlotClass);
	}

	if (QuickSlots[FromQuickSlotIndex]->MoveLastItemsCountToOtherSlot(Count, QuickSlots[ToQuickSlotIndex]))
	{
		if (QuickSlots[FromQuickSlotIndex]->IsEmpty() && ClearIfEmpty)
		{
			QuickSlots[FromQuickSlotIndex]->Clear();
		}

		OnQuickSlotChangedBind.Broadcast(FromQuickSlotIndex);
		OnQuickSlotChangedBind.Broadcast(ToQuickSlotIndex);
	}
}

void UJT_InventoryComponent::MoveItemsFromQuickSlotToInventory_Implementation(int QuickSlotIndex, int Count, bool MustBeNewSlot, bool ClearIfEmpty)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || !IsValid(QuickSlots[QuickSlotIndex]) || QuickSlots[QuickSlotIndex]->IsEmpty()) return;


	if (QuickSlots[QuickSlotIndex]->MoveLastItemsCountToInventory(Count, MustBeNewSlot, this))
	{
		if (QuickSlots[QuickSlotIndex]->IsEmpty() && ClearIfEmpty)
		{
			QuickSlots[QuickSlotIndex]->Clear();
		}

		OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
	}
}

void UJT_InventoryComponent::MoveAllItemsFromQuickSlotsToInventory_Implementation(bool MustBeNewSlot, bool ClearIfEmpty)
{
	for (int i = 0; i < QuickSlots.Num(); ++i)
	{
		if (!IsValid(QuickSlots[i])) continue;

		MoveItemsFromQuickSlotToInventory_Implementation(i, QuickSlots[i]->GetItemsCount(), MustBeNewSlot, ClearIfEmpty);
	}
}

void UJT_InventoryComponent::MoveItemsFromInventoryToQuickSlot_Implementation(int InventorySlotIndex, int QuickSlotIndex, int Count, bool ApplyDeleteChanges)
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex) || !Inventory.IsValidIndex(InventorySlotIndex) || Inventory[InventorySlotIndex]->IsEmpty()) return;


	if (!IsValid(QuickSlots[QuickSlotIndex]) || QuickSlots[QuickSlotIndex]->GetClass() == UJT_InventorySlotBase::StaticClass())
	{
		QuickSlots[QuickSlotIndex] = NewObject<UJT_InventorySlotBase>(this, Inventory[InventorySlotIndex]->GetFirstItem()->InventorySlotClass);
	}

	if (Inventory[InventorySlotIndex]->MoveLastItemsCountToOtherSlot(Count, QuickSlots[QuickSlotIndex]))
	{
		if (ApplyDeleteChanges) ApplySlotDeleteChanges(InventorySlotIndex);

		OnQuickSlotChangedBind.Broadcast(QuickSlotIndex);
	}
}

void UJT_InventoryComponent::MoveItemsFromInventoryToAnyQuickSlot_Implementation(int InventorySlotIndex, int Count, bool ShouldBeNotEmpty, bool ApplyDeleteChanges)
{
	if (!Inventory.IsValidIndex(InventorySlotIndex) || Count <= 0 || Inventory[InventorySlotIndex]->IsEmpty()) return;


	for (int i = 0; i < QuickSlots.Num() && Count > 0; ++i)
	{
		if (!IsValid(QuickSlots[i]) || QuickSlots[i]->GetClass() == UJT_InventorySlotBase::StaticClass() || QuickSlots[i]->IsEmpty())
		{
			if (ShouldBeNotEmpty)
			{
				continue;
			}
			else
			{
				QuickSlots[i] = NewObject<UJT_InventorySlotBase>(this, Inventory[InventorySlotIndex]->GetFirstItem()->InventorySlotClass);
			}
		}


		int LItemsCount = Inventory[InventorySlotIndex]->GetItemsCount();
		if (Inventory[InventorySlotIndex]->MoveLastItemsCountToOtherSlot(Count, QuickSlots[i]))
		{
			Count = Count - (LItemsCount - Inventory[InventorySlotIndex]->GetItemsCount());
			if (ApplyDeleteChanges) ApplySlotDeleteChanges(InventorySlotIndex);

			OnQuickSlotChangedBind.Broadcast(i);
		}
	}
}

bool UJT_InventoryComponent::GetIsQuickSlotEmpty(int QuickSlotIndex) const
{
	if (!QuickSlots.IsValidIndex(QuickSlotIndex)) return true;

	return QuickSlots[QuickSlotIndex]->IsEmpty();
}

//....................................................................................................................................//












FTransform UJT_InventoryComponent::GetDroppingItemTransform() const
{
	FRotator LSpawnRotation = MyActor->GetActorRotation();
	FVector LSpawnLocation = MyActor->GetActorLocation() + (MyActor->GetActorForwardVector() * DropFromInventoryDistance) + (MyActor->GetActorRightVector() * DropFromInventoryRightOffset);

	return FTransform(LSpawnRotation, LSpawnLocation, FVector(1, 1, 1));
}
