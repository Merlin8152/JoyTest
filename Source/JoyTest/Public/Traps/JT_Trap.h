// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JT_Trap.generated.h"

UCLASS(Abstract)
class JOYTEST_API AJT_Trap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJT_Trap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Trap")
		void OnStartTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnStartTrigger_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Trap")
		void OnEndTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void OnEndTrigger_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
		float Impulse = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
		float Damage = 15.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
		bool IsDestroyOnTrigger = true;


};
