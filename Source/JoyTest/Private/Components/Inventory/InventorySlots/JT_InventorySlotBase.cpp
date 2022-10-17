// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"


#include "Components/Inventory/JT_InventoryComponent.h"
#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"






UJT_InventorySlotBase::UJT_InventorySlotBase()
{
}





UWorld* UJT_InventorySlotBase::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld) return nullptr;

	if (GetOuter()) return GetOuter()->GetWorld();

	return nullptr;
}


void UJT_InventorySlotBase::PostInitProperties()
{
	Super::PostInitProperties();

	MyInventoryComponent = Cast<UJT_InventoryComponent>(GetOuter());
}







bool UJT_InventorySlotBase::IsEmpty_Implementation() const
{
	return GetItemsCount() <= 0;
}

bool UJT_InventorySlotBase::IsFull_Implementation() const
{
	return !IsEmpty() && GetItemsCount() >= GetFirstItem()->MaxInStack;
}

bool UJT_InventorySlotBase::WillFull_Implementation(int IfAddCount, int DefaultMaxInSlot) const
{
	int TempNum = GetItemsCount() + IfAddCount;
	return IsEmpty() ? TempNum >= DefaultMaxInSlot : TempNum >= GetFirstItem()->MaxInStack;
}

int UJT_InventorySlotBase::GetItemsCount_Implementation() const
{
	return ItemsInfo.Num();
}

UJT_InventoryItemInfo* UJT_InventorySlotBase::GetFirstItem_Implementation() const
{
	if (IsEmpty()) return nullptr;

	return ItemsInfo[0];
}






bool UJT_InventorySlotBase::IsEqual_Implementation(UJT_InventorySlotBase* OtherSlot) const
{
	if (!IsValid(OtherSlot)) return false;

	if (OtherSlot->IsEmpty() && IsEmpty()) return true;
	if (OtherSlot->IsEmpty() != IsEmpty()) return false;

	return OtherSlot->GetClass() == GetClass() && OtherSlot->GetFirstItem()->IsEqual(GetFirstItem());
}





bool UJT_InventorySlotBase::GetCanAddItem_Implementation(UJT_InventoryItemInfo* ItemInfo)
{
	if (!IsValid(ItemInfo) || IsFull() || !ItemInfo->IsInfoDataValid()) return false;

	if (IsEmpty()) return true;

	return ItemInfo->IsEqual(GetFirstItem());
}

bool UJT_InventorySlotBase::AddItem_Implementation(UJT_InventoryItemInfo* ItemInfo)
{
	verify(IsValid(MyInventoryComponent));


	if (!GetCanAddItem(ItemInfo)) return false;


	ItemsInfo.Add(ItemInfo);

	return true;
}

int UJT_InventorySlotBase::RemoveLastItemsCount_Implementation(int Count)
{
	verify(IsValid(MyInventoryComponent));

	if (Count < 0 || IsEmpty()) return 0;

	Count = FMath::Min(Count, GetItemsCount());

	for (int i = 0; i < Count; ++i)
	{
		ItemsInfo.Pop(true);
	}

	return Count;
}

bool UJT_InventorySlotBase::RemoveLastItem_Implementation()
{
	verify(IsValid(MyInventoryComponent));
	if (IsEmpty()) return false;

	ItemsInfo.Pop(true);

	return true;
}

int UJT_InventorySlotBase::DropLastItemsCount_Implementation(int Count, bool SimulatePhysics, const FTransform& SpawnTransform)
{
	verify(IsValid(MyInventoryComponent));
	if (IsEmpty() || Count <= 0) return 0;



	Count = FMath::Min<int>(Count, GetItemsCount());

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnInfo.OverrideLevel = MyInventoryComponent->MyActor->GetLevel();

	for (int i = 0; i < Count; ++i)
	{
		UJT_InventoryItemInfo* LItemToDrop = ItemsInfo[GetItemsCount() - i - 1];

		AActor* SpawnedActor = GetWorld()->SpawnActor(LItemToDrop->ItemClass, &SpawnTransform, SpawnInfo);

		RemoveLastItem();
		MyInventoryComponent->OnItemDroppedBind.Broadcast(SpawnedActor);
	}

	return Count;
}

void UJT_InventorySlotBase::Clear_Implementation()
{
	verify(IsValid(MyInventoryComponent));

	ItemsInfo.Empty();
}

bool UJT_InventorySlotBase::MoveLastItemsCountToOtherSlot_Implementation(int Count, UJT_InventorySlotBase* OtherSlot)
{
	verify(IsValid(MyInventoryComponent));
	if (Count <= 0 || !IsValid(OtherSlot) || OtherSlot == this || IsEmpty()) return false;

	Count = FMath::Min<int>(GetItemsCount(), Count);

	int MovedCount = 0;
	for (int i = 0; i < Count; ++i)
	{
		if (OtherSlot->AddItem(ItemsInfo[GetItemsCount() - i - 1]))
		{
			++MovedCount;
		}
		else
		{
			break;
		}
	}

	if (MovedCount <= 0) return false;

	RemoveLastItemsCount(MovedCount);

	return true;
}

bool UJT_InventorySlotBase::MoveLastItemsCountToInventory_Implementation(int Count, bool MustBeNewSlot, UJT_InventoryComponent* OtherInventory)
{
	verify(IsValid(MyInventoryComponent));
	if (
		Count <= 0 || !IsValid(OtherInventory) || IsEmpty()
		) return false;

	Count = FMath::Min<int>(GetItemsCount(), Count);


	int CanMoveCount = OtherInventory->GetItemsCountWhichCouldBeAddedToInventory(GetFirstItem(), MustBeNewSlot);
	if (CanMoveCount <= 0) return false;

	Count = FMath::Min<int>(CanMoveCount, Count);

	TArray<UJT_InventoryItemInfo*> ItemsToMove;
	for (int i = 0; i < Count; ++i)
	{
		ItemsToMove.Add(ItemsInfo[GetItemsCount() - i - 1]);
	}

	OtherInventory->AddItemsToInventory(ItemsToMove, MustBeNewSlot);
	RemoveLastItemsCount(Count);


	return true;

}
