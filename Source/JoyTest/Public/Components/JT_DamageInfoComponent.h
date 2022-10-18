// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JT_DamageInfoComponent.generated.h"


class AJT_DamageInfoActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JOYTEST_API UJT_DamageInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJT_DamageInfoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void OnActorTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

public:
	/*Created widget class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AJT_DamageInfoActor> SpawnActorClass;

};
