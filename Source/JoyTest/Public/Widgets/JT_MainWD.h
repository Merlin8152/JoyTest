// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_MainWD.generated.h"

class AJT_CharacterBase;

class UTextBlock;
class UProgressBar;
/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API UJT_MainWD : public UUserWidget
{
	GENERATED_BODY()


	UJT_MainWD(const FObjectInitializer& ObjectInitializer);

	//c++ protected methods
protected:

	virtual void NativeDestruct() override;




	//>>.............................................................................................................................................<<//



	//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MainWD")
		void InitializeMainWD(AJT_CharacterBase* PlayerCharacter);
	virtual void InitializeMainWD_Implementation(AJT_CharacterBase* PlayerCharacter);



	UFUNCTION(BlueprintCallable, Category = "MainWD")
		void UpdateHP(float NewHP, float MaxHP);


	UFUNCTION(BlueprintCallable)
		void SetInteractiveInfo(const FText& InfoText);
	UFUNCTION(BlueprintCallable)
		void HideInteractiveInfo();


	//Blueprint values
public:

	/*
		Progress bar for current player's HP.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainWD|Widgets")
		UProgressBar* HPBar = nullptr;
	/*
		Text block for InteractiveInfo.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainWD|Widgets")
		UTextBlock* InteractiveInfoText = nullptr;



	UPROPERTY(BlueprintReadOnly)
		AJT_CharacterBase* MyPlayerCharacter = nullptr;

};
