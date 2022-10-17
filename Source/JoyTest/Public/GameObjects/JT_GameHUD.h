// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "JT_GameHUD.generated.h"

class UJT_MainWD;
class UJT_MainMenuWD;
class UJT_InventoryWD;
class UJT_DeathMenu;

class AJT_PlayerController;
class UJT_GameInstance;

UENUM()
enum class EWidgetFocusType : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI
};



/**
 *
 */
UCLASS()
class JOYTEST_API AJT_GameHUD : public AHUD
{
	GENERATED_BODY()

		AJT_GameHUD();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	//c++ protected methods
protected:

	virtual void BeginPlay() override;
public:
	void SetFocusToWidget(EWidgetFocusType WidgetFocusType, UUserWidget* Widget);
	virtual void DrawHUD() override;
	//Blueprint methods
public:


	UFUNCTION(BlueprintCallable, Category = "MainWD")
		void CreateMainHUD();
	UFUNCTION(BlueprintCallable, Category = "MainWD")
		void RemoveMainHUD();

	UFUNCTION(BlueprintCallable, Category = "MainWD|InteractiveInfo")
		void ShowInteractiveInfo(const FText& InteractiveInfo);
	UFUNCTION(BlueprintCallable, Category = "MainWD|InteractiveInfo")
		void HideInteractiveInfo();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWD")
		void CreateMainMenuWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenuWD")
		void SetFocusToMainMenuWD();
	UFUNCTION(BlueprintCallable, Category = "MainMenuWD")
		void RemoveMainMenuWD();

	UFUNCTION(BlueprintCallable, Category = "DeathMenuWD")
		void CreateDeathMenuWD();
	UFUNCTION(BlueprintCallable, Category = "DeathMenuWD")
		void SetFocusToDeathMenuWD();
	UFUNCTION(BlueprintCallable, Category = "DeathMenuWD")
		void RemoveDeathMenuWD();


	UFUNCTION(BlueprintCallable, Category = "InventoryWD")
		void CreateInventoryWD();
	UFUNCTION(BlueprintCallable, Category = "InventoryWD")
		void SetFocusToInventoryWD();
	UFUNCTION(BlueprintCallable, Category = "InventoryWD")
		void RemoveInventoryWD();

	UFUNCTION(BlueprintCallable, Category = "GameplayActions")
		bool RemoveAllWidgets();
	UFUNCTION(BlueprintCallable, Category = "GameplayActions")
		bool QuitGame();
	UFUNCTION(BlueprintCallable, Category = "GameplayActions")
		void OnRespawn();
	UFUNCTION(BlueprintCallable, Category = "GameplayActions")
		bool OpenGame(const FString& MapName);
	//Blueprint values
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSoftClassPtr<UJT_MainWD> MainWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UJT_MainWD* MainWD = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSoftClassPtr<UJT_MainMenuWD> MainMenuWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UJT_MainMenuWD* MainMenuWD = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSoftClassPtr<UJT_InventoryWD> InventoryWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UJT_InventoryWD* InventoryWD = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSoftClassPtr<UJT_DeathMenu> DeathMenuWDClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
		UJT_DeathMenu* DeathMenuWD = nullptr;


	UPROPERTY(BlueprintReadOnly)
		AJT_PlayerController* MyController = nullptr;
	UPROPERTY(BlueprintReadOnly)
		UJT_GameInstance* GameInstance = nullptr;
};
