// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_DragDropSlotWD.generated.h"

class UImage;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_DragDropSlotWD : public UUserWidget
{
	GENERATED_BODY()



	//Blueprint variables
public:

	/*
		Image for Drag n Drop slot Icon
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DragDropSlotWD | Widgets")
		UImage* DragDropSlotIcon = nullptr;
};
