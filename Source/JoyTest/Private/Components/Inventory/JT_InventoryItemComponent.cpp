// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/JT_InventoryItemComponent.h"
#include "Components/Inventory/ItemInfo/JT_InventoryItemInfo.h"





UJT_InventoryItemComponent::UJT_InventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}




void UJT_InventoryItemComponent::BeginPlay()
{
	Super::BeginPlay();

	//TODO on child
}

void UJT_InventoryItemComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	//TODO on child
}

void UJT_InventoryItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Not available on this component
}






void UJT_InventoryItemComponent::SimulatePhysicsServertToAll_Implementation(bool Simulate)
{
	if( GetOwner()->GetLocalRole() < ROLE_Authority ) return;

	OnSimulatePhysics(Simulate);
	OnSimulatePhysicsBind.Broadcast(Simulate);
}

void UJT_InventoryItemComponent::SetMasterPoseServertToAll_Implementation(USkeletalMeshComponent* MasterComp)
{
	if( GetOwner()->GetLocalRole() < ROLE_Authority ) return;

	OnSetMasterPose(MasterComp);
	OnSetMasterPoseBind.Broadcast(MasterComp);
}

void UJT_InventoryItemComponent::ApplyMorphServertToAll_Implementation(const FName& MorphName, float Value, bool RemoveZeroWeight)
{
	if( GetOwner()->GetLocalRole() < ROLE_Authority ) return;

	OnApplyMorph(MorphName, Value, RemoveZeroWeight);
	OnApplyMorphBind.Broadcast(MorphName, Value, RemoveZeroWeight);
}

void UJT_InventoryItemComponent::ClearAllMorphsServertToAll_Implementation()
{
	if( GetOwner()->GetLocalRole() < ROLE_Authority ) return;

	OnClearAllMorphs();
	OnClearAllMorphsBind.Broadcast();
}

