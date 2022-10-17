// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/JT_MineTrap.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AJT_MineTrap::AJT_MineTrap()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AJT_MineTrap::OnStartTrigger);
}

void AJT_MineTrap::OnStartTrigger_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation(), GetActorRotation());
	}

	Super::OnStartTrigger_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

