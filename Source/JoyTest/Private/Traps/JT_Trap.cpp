// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/JT_Trap.h"

// Sets default values
AJT_Trap::AJT_Trap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AJT_Trap::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJT_Trap::OnStartTrigger_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float TakedDamage = OtherActor->TakeDamage(Damage, FDamageEvent(), nullptr, this);
	// Only add impulse if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation((GetActorLocation() - OtherActor->GetActorLocation()) * Impulse, GetActorLocation());
	}

	if (IsDestroyOnTrigger) Destroy();
}


