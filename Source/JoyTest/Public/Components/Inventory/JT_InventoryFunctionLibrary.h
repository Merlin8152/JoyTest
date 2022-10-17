// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JT_InventoryFunctionLibrary.generated.h"




class UJT_InventoryItemInfo;
class UJT_InventoryItemComponent;
class UJT_InventorySlotBase;




UCLASS()
class JOYTEST_API UJT_InventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* 
		Find item info in actor.
		@return nullptr if wrong actor.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static UJT_InventoryItemInfo* TryGetItemInfoFromActor(AActor* Actor);
	/*
		Find item info in actor and copy it.
		@return Copy of item info. 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static UJT_InventoryItemInfo* TryGetItemInfoCopyFromActor(AActor* Actor);
	/*
		Find item info in actor class and copy it.
		@return Copy of item info.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static UJT_InventoryItemInfo* TryGetItemInfoCopyFromActorClass(TSubclassOf<AActor> ActorClass);

	/*
		Find InventoryItemComponent in actor class.
		@return nullptr if wrong actor class.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static UJT_InventoryItemComponent* FindInventoryItemComponentInClass(TSubclassOf<AActor> ActorClass);
	

	/*
		Checks two slots for equality.
		@return true if (InventorySlotA is equal to InventorySlotB) and (InventorySlotB is equal to InventorySlotA). 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static bool AreTwoSlotsEqual(UJT_InventorySlotBase* InventorySlotA, UJT_InventorySlotBase* InventorySlotB);
	/*
		Checks two items for equality.
		@return true if (ItemInfoA is equal to ItemInfoB) and (ItemInfoB is equal to ItemInfoA). 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PZ_InventoryFunctionLibrary")
		static bool AreTwoItemsInfoIdentical(UJT_InventoryItemInfo* ItemInfoA, UJT_InventoryItemInfo* ItemInfoB);
};
