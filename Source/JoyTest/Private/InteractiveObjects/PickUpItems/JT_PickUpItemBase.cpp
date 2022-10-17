// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjects/PickUpItems/JT_PickUpItemBase.h"

#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"

#include "Components/Inventory/JT_InventoryComponent.h"

// Sets default values
AJT_PickUpItemBase::AJT_PickUpItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractionMessage = FText::FromString("Pickup");
}



void AJT_PickUpItemBase::BeginPlay()
{
	Super::BeginPlay();
}



void AJT_PickUpItemBase::OnActivatedForInteract_Implementation(AActor* InstigatedBy)
{
	if (!IsValid(InstigatedBy)) return;

	if (UJT_InventoryComponent* LInventoryComp = InstigatedBy->FindComponentByClass<UJT_InventoryComponent>())
	{
		if (LInventoryComp->GetCanCollectItem(ItemInfo))
		{
			LInventoryComp->CollectItem(ItemInfo);

			Destroy();
		}
	}
}


