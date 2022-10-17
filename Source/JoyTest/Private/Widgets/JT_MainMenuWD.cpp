// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JT_MainMenuWD.h"
#include "GameObjects/JT_GameHUD.h"
#include "Components/Button.h"



UJT_MainMenuWD::UJT_MainMenuWD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UJT_MainMenuWD::NativeConstruct()
{
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UJT_MainMenuWD::OnExitButtonClicked);
	if (StartButton) StartButton->OnClicked.AddDynamic(this, &UJT_MainMenuWD::OnStartButtonClicked);
}

void UJT_MainMenuWD::InitializeMainMenuWD_Implementation(AJT_GameHUD* InGameHUD)
{
	if (IsValid(InGameHUD)) MyGameHUD = InGameHUD;
}



void UJT_MainMenuWD::OnStartButtonClicked()
{
	if (!IsValid(MyGameHUD)) return;

	MyGameHUD->OpenGame(StartMapName);
}

void UJT_MainMenuWD::OnExitButtonClicked()
{
	if (IsValid(MyGameHUD)) MyGameHUD->QuitGame();
}

