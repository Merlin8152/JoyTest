// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/JT_InteractiveObjectBase.h"
#include "JT_PickUpItemBase.generated.h"

class UJT_InventoryItemInfo;

UCLASS(Abstract)
class JOYTEST_API AJT_PickUpItemBase : public AJT_InteractiveObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJT_PickUpItemBase();



	// c++ protected methods
protected:

	virtual void BeginPlay() override;



	//........................................................................................................................................//



	//Blueprints methods
public:

	virtual void OnActivatedForInteract_Implementation(AActor* InstigatedBy) override;



	//Blueprints values
public:

	/*
	Main information about this item.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
		UJT_InventoryItemInfo* ItemInfo = nullptr;
};
