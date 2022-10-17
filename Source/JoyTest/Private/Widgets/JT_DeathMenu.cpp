// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JT_DeathMenu.h"

#include "GameObjects/JT_GameHUD.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UJT_DeathMenu::UJT_DeathMenu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UJT_DeathMenu::NativeConstruct()
{
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UJT_DeathMenu::OnExitButtonClicked);
	if (RespawnButton) RespawnButton->OnClicked.AddDynamic(this, &UJT_DeathMenu::OnRespawnButtonClicked);
}

void UJT_DeathMenu::OnRespawnButtonClicked()
{
	if (IsValid(MyGameHUD)) MyGameHUD->OnRespawn();
}

void UJT_DeathMenu::OnExitButtonClicked()
{
	if (IsValid(MyGameHUD)) MyGameHUD->QuitGame();
}

void UJT_DeathMenu::OnTimerUpdate()
{
	CurrentTime -= RespawnTimerUpdate;
	FMath::Max(0.0f, CurrentTime);

	if (CurrentTime == 0.0f) OnRespawnTimeEnd();

	if (TimerText) TimerText->SetText(FText::FromString(FString::SanitizeFloat(CurrentTime)));
}

void UJT_DeathMenu::OnRespawnTimeEnd()
{
	if (RespawnButton) RespawnButton->SetIsEnabled(true);
	MyGameHUD->GetWorldTimerManager().ClearTimer(RespawnUpdateTimer);
}

void UJT_DeathMenu::InitializeDeathWD_Implementation(AJT_GameHUD* InGameHUD)
{
	if (!IsValid(InGameHUD)) return;

	MyGameHUD = InGameHUD;

	CurrentTime = RespawnTime;
	if (RespawnButton) RespawnButton->SetIsEnabled(false);
	if (TimerText) TimerText->SetText(FText::FromString(FString::SanitizeFloat(CurrentTime)));
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("OnTimerUpdate"));
	MyGameHUD->GetWorldTimerManager().SetTimer(RespawnUpdateTimer, TimerDel, RespawnTimerUpdate, true);
}
