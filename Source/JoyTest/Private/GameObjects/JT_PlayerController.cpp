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



FVector2D AJT_PlayerController::GetViewportHalfSize() const
{
	int32 XSize, YSize;
	GetViewportSize(XSize, YSize);

	return FVector2D(XSize / 2, YSize / 2);
}

bool AJT_PlayerController::GetHitResultAtScreenRadius(const FVector2D& ScreenPosition, float Radius, ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const
{
	if (GetHUD() != nullptr && GetHUD()->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}


	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), bTraceComplex);

		return GetWorld()->SweepSingleByChannel
		(
			HitResult,
			WorldOrigin,
			WorldOrigin + WorldDirection * HitResultTraceDistance,
			FQuat::Identity,
			TraceChannel,
			FCollisionShape::MakeSphere(Radius),
			FCollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), bTraceComplex)
		);
	}

	return false;
}
