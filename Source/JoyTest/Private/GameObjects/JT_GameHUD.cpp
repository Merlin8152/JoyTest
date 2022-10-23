// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/JT_GameHUD.h"

#include "GameObjects/JT_PlayerController.h"
#include "GameObjects/JT_GameInstance.h"
#include "Character/JT_CharacterBase.h"

#include "Widgets/JT_MainWD.h"
#include "Widgets/JT_MainMenuWD.h"
#include "Widgets/JT_InventoryWD.h"
#include "Widgets/JT_DeathMenu.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"

AJT_GameHUD::AJT_GameHUD()
{
	// Set the crosshair texture
	//static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	//CrosshairTex = CrosshairTexObj.Object;
}

void AJT_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AJT_PlayerController>(GetOwningPlayerController());
	GameInstance = Cast<UJT_GameInstance>(GetGameInstance());
}

void AJT_GameHUD::SetFocusToWidget(EWidgetFocusType WidgetFocusType, UUserWidget* Widget)
{
	if (!IsValid(Widget)) return;


	switch (WidgetFocusType)
	{
	case EWidgetFocusType::GameOnly:
	{
		FInputModeGameOnly LNewGameOnlyInput;
		MyController->SetInputMode(LNewGameOnlyInput);
		break;
	}

	case EWidgetFocusType::UIOnly:
	{
		FInputModeUIOnly LNewUIOnlyInput;
		LNewUIOnlyInput.SetWidgetToFocus(Widget->TakeWidget());
		MyController->SetInputMode(LNewUIOnlyInput);
		break;
	}

	case EWidgetFocusType::GameAndUI:
	{
		FInputModeGameAndUI LNewGameAndUIInput;
		LNewGameAndUIInput.SetWidgetToFocus(Widget->TakeWidget());
		MyController->SetInputMode(LNewGameAndUIInput);
		break;
	}
	}

	if (Widget->bIsFocusable) Widget->SetKeyboardFocus();
}

void AJT_GameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!IsDrawCrosshair || !CrosshairTex) return;

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X),
		(Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}




//.................................MainWD.......................................................//

void AJT_GameHUD::CreateMainHUD()
{
	if (!MainWDClass.IsNull() && !MainWD)
	{
		MainWD = CreateWidget<UJT_MainWD>(GetWorld(), MainWDClass.LoadSynchronous());
		if (MainWD) MainWD->InitializeMainWD(MyController->MyCharacter);
		if (MainWD) MainWD->AddToViewport();
	}
}

void AJT_GameHUD::RemoveMainHUD()
{
	if (MainWD)
	{
		MainWD->RemoveFromParent();
		MainWD = nullptr;
	}
}

void AJT_GameHUD::ShowInteractiveInfo(const FText& InteractiveInfo)
{
	if (!MainWD) return;
	
	MainWD->SetInteractiveInfo(InteractiveInfo);
}

void AJT_GameHUD::HideInteractiveInfo()
{
	if (!MainWD) return;

	MainWD->HideInteractiveInfo();
}


//...............................................................................................//

void AJT_GameHUD::CreateMainMenuWD()
{
	if (!MainMenuWDClass.IsNull() && !MainMenuWD)
	{
		MainMenuWD = CreateWidget<UJT_MainMenuWD>(GetWorld(), MainMenuWDClass.LoadSynchronous());
		if (MainMenuWD) MainMenuWD->InitializeMainMenuWD(this);
		if (MainMenuWD) MainMenuWD->AddToViewport();
	}

	SetFocusToMainMenuWD();
}

void AJT_GameHUD::SetFocusToMainMenuWD()
{
	if (MainMenuWD)
	{
		MyController->SetShowMouseCursor(true);

		SetFocusToWidget(EWidgetFocusType::UIOnly, MainMenuWD);
	}
}

void AJT_GameHUD::RemoveMainMenuWD()
{
	if (MainMenuWD)
	{
		MainMenuWD->RemoveFromParent();
		MainMenuWD = nullptr;

		MyController->SetShowMouseCursor(false);
		MyController->SetInputMode(FInputModeGameOnly());
	}
}


//...............................................................................................//

void AJT_GameHUD::CreateDeathMenuWD()
{
	if (!DeathMenuWDClass.IsNull() && !DeathMenuWD)
	{

		DeathMenuWD = CreateWidget<UJT_DeathMenu>(GetWorld(), DeathMenuWDClass.LoadSynchronous());
		if (DeathMenuWD) DeathMenuWD->InitializeDeathWD(this);
		if (DeathMenuWD) DeathMenuWD->AddToViewport();
	}

	SetFocusToDeathMenuWD();
}

void AJT_GameHUD::SetFocusToDeathMenuWD()
{
	if (DeathMenuWD)
	{
		MyController->SetShowMouseCursor(true);

		SetFocusToWidget(EWidgetFocusType::UIOnly, DeathMenuWD);
	}
}

void AJT_GameHUD::RemoveDeathMenuWD()
{
	if (DeathMenuWD)
	{
		DeathMenuWD->RemoveFromParent();
		DeathMenuWD = nullptr;

		MyController->SetShowMouseCursor(false);
		MyController->SetInputMode(FInputModeGameOnly());
	}
}

//...............................................................................................//


void AJT_GameHUD::CreateInventoryWD()
{
	if (!InventoryWDClass.IsNull() && !InventoryWD)
	{

		InventoryWD = CreateWidget<UJT_InventoryWD>(GetWorld(), InventoryWDClass.LoadSynchronous());
		if (InventoryWD) InventoryWD->InitializeInventoryWD(MyController->MyCharacter->GetInventoryComponent(), this);
		if (InventoryWD) InventoryWD->AddToViewport();
	}

	MyController->SetPause(true);
	SetFocusToInventoryWD();
}

void AJT_GameHUD::SetFocusToInventoryWD()
{
	if (InventoryWD)
	{
		MyController->SetShowMouseCursor(true);

		SetFocusToWidget(EWidgetFocusType::UIOnly, InventoryWD);
	}
}

void AJT_GameHUD::RemoveInventoryWD()
{
	if (InventoryWD)
	{
		InventoryWD->RemoveFromParent();
		InventoryWD = nullptr;

		MyController->SetPause(false);

		MyController->SetShowMouseCursor(false);
		MyController->SetInputMode(FInputModeGameOnly());
	}
}

//...............................................................................................//


bool AJT_GameHUD::RemoveAllWidgets()
{
	RemoveInventoryWD();
	RemoveMainHUD();
	RemoveMainMenuWD();
	RemoveDeathMenuWD();

	return true;
}

bool AJT_GameHUD::QuitGame()
{
	MyController->ConsoleCommand("quit");

	return true;
}

void AJT_GameHUD::OnRespawn()
{
	GameInstance->OnRespawn();

	OpenGame(GameInstance->CurrentMapName);
}

bool AJT_GameHUD::OpenGame(const FString& MapName)
{
	if (GameInstance->OpenGame(MapName))
	{
		MyController->SetShowMouseCursor(false);
		MyController->SetInputMode(FInputModeGameOnly());

		return true;
	}

	return false;
}
