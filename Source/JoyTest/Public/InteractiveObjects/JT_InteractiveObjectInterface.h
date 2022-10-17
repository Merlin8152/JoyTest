// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JT_InteractiveObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJT_InteractiveObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JOYTEST_API IJT_InteractiveObjectInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractiveObject")
		void OnActivatedForInteract(AActor* InstigatedBy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractiveObject")
		FText GetInteractiveMessage();
};
