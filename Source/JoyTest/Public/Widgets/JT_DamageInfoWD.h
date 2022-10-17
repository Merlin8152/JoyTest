// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_DamageInfoWD.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_DamageInfoWD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MainWD")
		void SetDamageText(const FText& Text);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainWD|Widgets")
		UTextBlock* DamageText = nullptr;


};
