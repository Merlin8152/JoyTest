// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_MainMenuWD.generated.h"


class UButton;
class AJT_GameHUD;
/**
 * 
 */
UCLASS(Abstract)
class JOYTEST_API UJT_MainMenuWD : public UUserWidget
{
	GENERATED_BODY()

		//constructor
public:

	UJT_MainMenuWD(const FObjectInitializer& ObjectInitializer);



	//c++ private methods
private:



	//c++ protected methods
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnStartButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();



	//c++ private values



	//>>..........................................................................................................................<<//



	//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MainMenuWD")
		void InitializeMainMenuWD(AJT_GameHUD* InGameHUD);
	virtual void InitializeMainMenuWD_Implementation(AJT_GameHUD* InGameHUD);

	//Blueprint values
public:

	/*
		Button for start game.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* StartButton = nullptr;
	/*
		Button for game exit.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "MainMenuWD|Widgets")
		UButton* ExitButton = nullptr;

	/*
	Name of the map to load.
*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainMenuWD")
		FString StartMapName;

	UPROPERTY(BlueprintReadOnly)
		AJT_GameHUD* MyGameHUD = nullptr;
};
