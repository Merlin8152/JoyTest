// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/JT_Trap.h"
#include "JT_MineTrap.generated.h"

class USphereComponent;
class UParticleSystem;

/**
 *
 */
UCLASS(Abstract)
class JOYTEST_API AJT_MineTrap : public AJT_Trap
{
	GENERATED_BODY()

	AJT_MineTrap();
public:


	virtual void OnStartTrigger_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USphereComponent* SphereComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ParticleSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* TriggerSound;
};
