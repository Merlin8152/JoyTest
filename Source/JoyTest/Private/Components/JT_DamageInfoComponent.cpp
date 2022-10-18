// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/JT_DamageInfoComponent.h"

#include "GameObjects/JT_DamageInfoActor.h"

// Sets default values for this component's properties
UJT_DamageInfoComponent::UJT_DamageInfoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UJT_DamageInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakePointDamage.AddDynamic(this, &UJT_DamageInfoComponent::OnActorTakeDamage);
}

void UJT_DamageInfoComponent::OnActorTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (SpawnActorClass)
	{
		/*Show damage*/
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		FTransform Transform = FTransform(GetOwner()->GetActorRotation(), HitLocation, FVector(1.0f, 1.0f, 1.0f));
		AJT_DamageInfoActor* DamageActor = GetWorld()->SpawnActor<AJT_DamageInfoActor>(SpawnActorClass, Transform);
		DamageActor->SetDamageText(FText::FromString(FString::SanitizeFloat(Damage)));
	}
}

