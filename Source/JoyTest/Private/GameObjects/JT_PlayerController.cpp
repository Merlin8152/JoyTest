// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/JT_PlayerController.h"

#include "GameObjects/JT_GameHUD.h"
#include "Character/JT_CharacterBase.h"
#include "Kismet/GameplayStatics.h"

AJT_PlayerController::AJT_PlayerController()
{
}



void AJT_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyGameHUD = Cast<AJT_GameHUD>(GetHUD());
}

void AJT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APawn* LPawn = GetPawn();
	if (IsValid(LPawn))
	{
		LPawn->EnableInput(this);
	}

	MyCharacter = Cast<AJT_CharacterBase>(InPawn);
}

void AJT_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	MyCharacter = nullptr;
	MyGameHUD = nullptr;

}



void AJT_PlayerController::OnGameStart_Implementation()
{
	if (IsValid(MyGameHUD))
	{
		MyGameHUD->CreateMainHUD();
		MyGameHUD->SetIsDrawCrosshair(true);
	}

	APawn* LPawn = GetPawn();
	if (IsValid(LPawn))
	{
		LPawn->EnableInput(this);
	}
}
