// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JT_DragDropStructs.h"
#include "Blueprint/DragDropOperation.h"
#include "JT_InventoryDragDropOperation.generated.h"

/**
 *
 */
UCLASS()
class JOYTEST_API UJT_InventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()


	// C++ private variables
private:


	/*
		Contains drag drop data, such as item info, slot index, etc.
	*/
	UPROPERTY()
		FInventoryDragDropData Data;



	//>>>...............................................................................................................................................................................<<<//
	//>>>...............................................................................................................................................................................<<<//




	//Blueprint public methods
public:

	//..............................................Initializes................................................................//

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Drag Drop Operation")
		bool InitializesDragDropData(const FInventoryDragDropData& InDragDropData);
	bool InitializesDragDropData_Implementation(const FInventoryDragDropData& InDragDropData);

	//................................................................................................................................//




	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Drag Drop Operation")
		FInventoryDragDropData GetData() const;
	FInventoryDragDropData GetData_Implementation() const;


};
