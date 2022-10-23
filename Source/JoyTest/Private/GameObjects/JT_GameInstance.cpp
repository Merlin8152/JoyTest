// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/JT_GameInstance.h"

#include "Kismet/GameplayStatics.h"

#include "GameObjects/JT_PlayerController.h"
#include "GameObjects/JT_GameHUD.h"

void UJT_GameInstance::StartGame()
{
	IsGameStarted = true;
	OnGameStartedBind.Broadcast();
}

void UJT_GameInstance::OnMainMenuOpened()
{
	IsGameStarted = false;

	CurrentMapName = MainMenuMapName;

	MainMenuWasOpened = true;

	if (AJT_PlayerController* LPlayerController = Cast<AJT_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		LPlayerController->MyGameHUD->CreateMainMenuWD();
		LPlayerController->MyGameHUD->SetIsDrawCrosshair(false);
	}
}

void UJT_GameInstance::OnGameOpened()
{
	StartGame();
}

void UJT_GameInstance::OnRespawn()
{
	
}

bool UJT_GameInstance::OpenGame(const FString& MapName)
{
	IsGameStarted = false;
	CurrentMapName = MapName;

	FString Options = "";
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true, Options);

	return true;
}
