// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjects/PickUpItems/JT_PickUpItemBase.h"
#include "JT_UsableItemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class JOYTEST_API AJT_UsableItemBase : public AJT_PickUpItemBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UseBy(AActor* InActor) ;
	virtual void UseBy_Implementation(AActor* InActor) {};
	
};
