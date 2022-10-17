// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JT_InventoryItemComponent.generated.h"




class UJT_InventoryItemInfo;




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimulatePhysics,   bool, Simulate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetMasterPose,   USkeletalMeshComponent*, MasterComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnApplyMorph,   const FName&, MorphName,  float, Value,  bool, RemoveZeroWeight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClearAllMorphs);


/*
	Item actors with this component will be in inventory system.
	It implements base interface between item meshes and inventory/equipment components.
	
	@see UPZ_InventoryItemInfo.
*/
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOYTEST_API UJT_InventoryItemComponent : public UActorComponent
{
	GENERATED_BODY()


//Constructor
public:	

	UJT_InventoryItemComponent();



//c++ public methods
public:

	//UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~UActorComponent


//>>...........................................................................................................................<<//


//Blueprint protected methods
protected:

	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryItemComponent")
		void OnSimulatePhysics(bool Simulate);
		virtual void OnSimulatePhysics_Implementation(bool Simulate) {}

	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryItemComponent")
		void OnSetMasterPose(USkeletalMeshComponent* MasterComp);
		virtual void OnSetMasterPose_Implementation(USkeletalMeshComponent* MasterComp) {}

	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryItemComponent")
		void OnApplyMorph(const FName& MorphName, float Value, bool RemoveZeroWeight);
		virtual void OnApplyMorph_Implementation(const FName& MorphName, float Value, bool RemoveZeroWeight) {}

	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryItemComponent")
		void OnClearAllMorphs();
		virtual void OnClearAllMorphs_Implementation() {}

//Blueprint public methods
public:	

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "InventoryItemComponent")
		void SimulatePhysicsServertToAll(bool Simulate); //only on server
		void SimulatePhysicsServertToAll_Implementation(bool Simulate);
	UPROPERTY(BlueprintAssignable, Category = "InventoryItemComponent|EventsForBind")
		FOnSimulatePhysics OnSimulatePhysicsBind;


	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "InventoryItemComponent")
		void SetMasterPoseServertToAll(USkeletalMeshComponent* MasterComp); //only on server
		void SetMasterPoseServertToAll_Implementation(USkeletalMeshComponent* MasterComp);
	UPROPERTY(BlueprintAssignable, Category = "InventoryItemComponent|EventsForBind")
		FOnSetMasterPose OnSetMasterPoseBind;


	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "InventoryItemComponent")
		void ApplyMorphServertToAll(const FName& MorphName, float Value, bool RemoveZeroWeight); //only on server
		void ApplyMorphServertToAll_Implementation(const FName& MorphName, float Value, bool RemoveZeroWeight);
	UPROPERTY(BlueprintAssignable, Category = "InventoryItemComponent|EventsForBind")
		FOnApplyMorph OnApplyMorphBind;


	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "InventoryItemComponent")
		void ClearAllMorphsServertToAll(); //only on server
		void ClearAllMorphsServertToAll_Implementation();
	UPROPERTY(BlueprintAssignable, Category = "InventoryItemComponent|EventsForBind")
		FClearAllMorphs OnClearAllMorphsBind;




//Blueprint values
public:

	/*
		Main information about this item.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
		UJT_InventoryItemInfo* ItemInfo = nullptr;
};
