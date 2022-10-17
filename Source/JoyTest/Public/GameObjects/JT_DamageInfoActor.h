// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JT_DamageInfoActor.generated.h"

class UWidgetComponent;
class UJT_DamageInfoWD;
UCLASS(Abstract)
class JOYTEST_API AJT_DamageInfoActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJT_DamageInfoActor();

protected:

	FTimerHandle DestroyTimer;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Bind function
	UFUNCTION()
		void OnDestroyTimer();
public:
	// Time To Actor Destroy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float TimeToDestroy = 1.0f;
	
	/*Created widget class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UJT_DamageInfoWD> WidgetClass;
	/*Created widget Component*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UWidgetComponent* WidgetComponent;


	UFUNCTION(BlueprintCallable)
		void SetDamageText(const FText& Text);


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
