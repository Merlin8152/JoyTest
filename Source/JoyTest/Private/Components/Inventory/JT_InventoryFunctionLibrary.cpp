// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/JT_InventoryFunctionLibrary.h"

#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"
#include "Components/Inventory/JT_InventoryItemComponent.h"
#include "Components/Inventory/InventorySlots/JT_InventorySlotBase.h"

#include "Engine/SCS_Node.h"





UJT_InventoryItemInfo* UJT_InventoryFunctionLibrary::TryGetItemInfoFromActor(AActor* Actor)
{
	if( !IsValid(Actor) ) return nullptr;


	if( UJT_InventoryItemComponent* LItemComponent = Actor->FindComponentByClass<UJT_InventoryItemComponent>() )
	{
		return LItemComponent->ItemInfo;
	}

	return nullptr;
}

UJT_InventoryItemInfo* UJT_InventoryFunctionLibrary::TryGetItemInfoCopyFromActor(AActor* Actor)
{
	if( UJT_InventoryItemInfo* LItemInfo = TryGetItemInfoFromActor(Actor) )
	{
		return DuplicateObject<UJT_InventoryItemInfo>(LItemInfo, nullptr);
	}

	return nullptr;
}

UJT_InventoryItemInfo* UJT_InventoryFunctionLibrary::TryGetItemInfoCopyFromActorClass(TSubclassOf<AActor> ActorClass)
{
	if( !IsValid(ActorClass) ) return nullptr;


	if( UJT_InventoryItemComponent* LItemComponent = FindInventoryItemComponentInClass(ActorClass) )
	{
		return DuplicateObject<UJT_InventoryItemInfo>(LItemComponent->ItemInfo, nullptr);
	}

	return nullptr;
}



UJT_InventoryItemComponent* UJT_InventoryFunctionLibrary::FindInventoryItemComponentInClass(TSubclassOf<AActor> ActorClass)
{
	//for c++ classes
	if( UJT_InventoryItemComponent* DefaultItemComponent = ActorClass.GetDefaultObject()->FindComponentByClass<UJT_InventoryItemComponent>() )
	{
		return DefaultItemComponent;
	}

	//for bp classes
	if( UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass))
	{
		const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

		for( USCS_Node* Node : ActorBlueprintNodes )
		{
			if( UJT_InventoryItemComponent* LItemComponent = Cast<UJT_InventoryItemComponent>(Node->ComponentTemplate) )
			{
				return LItemComponent;
			}
		}
	}

	return nullptr;
}





bool UJT_InventoryFunctionLibrary::AreTwoSlotsEqual(UJT_InventorySlotBase* InventorySlotA, UJT_InventorySlotBase* InventorySlotB)
{
	if( !IsValid(InventorySlotA) || !IsValid(InventorySlotB) ) return false;

	return InventorySlotA->IsEqual(InventorySlotB);
}

bool UJT_InventoryFunctionLibrary::AreTwoItemsInfoIdentical(UJT_InventoryItemInfo* ItemInfoA, UJT_InventoryItemInfo* ItemInfoB)
{
	if( !IsValid(ItemInfoA) || !IsValid(ItemInfoB) ) return false;

	return ItemInfoA->IsEqual(ItemInfoB);
}