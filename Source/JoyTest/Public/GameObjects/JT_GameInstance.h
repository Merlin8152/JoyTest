// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JT_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarted);

/**
 *
 */
UCLASS()
class JOYTEST_API UJT_GameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	/*
		Flag. It tells us that we are in on the playable level. Not in main menu.
	*/
	bool IsGameStarted = false;

	//...................................Game............................................//
public:

	UFUNCTION(BlueprintCallable, Category = "GameStartup")
		void StartGame();
	UPROPERTY(BlueprintAssignable, Category = "GameStartup")
		FOnGameStarted OnGameStartedBind;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
		bool GetIsGameStarted() const { return IsGameStarted; }

	UFUNCTION(BlueprintCallable, Category = "GameStartup")
		bool OpenGame(const FString& MapName);

	UFUNCTION(BlueprintCallable, Category = "GameStartup")
		void OnMainMenuOpened();
	UFUNCTION(BlueprintCallable, Category = "GameStartup")
		void OnGameOpened();
	UFUNCTION(BlueprintCallable, Category = "GameStartup")
		void OnRespawn();
	/*
		Name of the current opened level.
	*/
	FString CurrentMapName;


	/*
		Flag. It tells us that we have already opened main menu.
	*/
	bool MainMenuWasOpened = false;
	/*
		Path to maps folder.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
		FString MapsPath = "/Maps/";

	/*
		Name of the main menu map.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
		FString MainMenuMapName = "MainMenu";
};
