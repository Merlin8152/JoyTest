// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JT_PlayerController.generated.h"


class AJT_CharacterBase;
class AJT_GameHUD;

/**
 *
 */
UCLASS()
class JOYTEST_API AJT_PlayerController : public APlayerController
{
	GENERATED_BODY()


	//constructor
public:

	AJT_PlayerController();



	//c++ protected methods
protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;


	//Blueprint protected methods
protected:






	//Blueprint values
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerController")
		FVector2D GetViewportHalfSize() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerController")
		bool GetHitResultAtScreenRadius(const FVector2D& ScreenPosition, float Radius, ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult) const;

	UFUNCTION(BlueprintNativeEvent, Category = "PlayerController")
		void OnGameStart();
	virtual void OnGameStart_Implementation();


	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
		AJT_CharacterBase* MyCharacter = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController")
		AJT_GameHUD* MyGameHUD = nullptr;
};
