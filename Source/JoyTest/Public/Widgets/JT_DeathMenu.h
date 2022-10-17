// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JT_DeathMenu.generated.h"

class AJT_GameHUD;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class JOYTEST_API UJT_DeathMenu : public UUserWidget
{
	GENERATED_BODY()

		//constructor
public:

	UJT_DeathMenu(const FObjectInitializer& ObjectInitializer);



	//c++ private methods
private:



	//c++ protected methods
protected:

	FTimerHandle RespawnUpdateTimer;

	float CurrentTime = 0.0f;

	virtual void NativeConstruct() override;

	/*BindEvents*/
	UFUNCTION()
		void OnRespawnButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();

	UFUNCTION()
		void OnTimerUpdate();
	UFUNCTION()
		void OnRespawnTimeEnd();
	/*~BindEvents*/

	//c++ private values



	//>>..........................................................................................................................<<//



	//Blueprint methods
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DeathWD")
		void InitializeDeathWD(AJT_GameHUD* InGameHUD);
	virtual void InitializeDeathWD_Implementation(AJT_GameHUD* InGameHUD);

	//Blueprint values
public:
	/*TimeToRespawn*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "DeathWD|Widgets")
		float RespawnTime = 10.0f;
	/*TimeToUpdate Timer text*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "DeathWD|Widgets")
		float RespawnTimerUpdate = 0.5f;

	/*
		Button for start game.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DeathWD|Widgets")
		UButton* RespawnButton = nullptr;
	/*
		Button for game exit.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DeathWD|Widgets")
		UButton* ExitButton = nullptr;
	/*Timer text*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DeathWD|Widgets")
		UTextBlock* TimerText = nullptr;
	/*HUD link*/
	UPROPERTY(BlueprintReadOnly)
		AJT_GameHUD* MyGameHUD = nullptr;
	
};
