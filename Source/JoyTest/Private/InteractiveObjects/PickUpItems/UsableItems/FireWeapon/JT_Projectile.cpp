// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjects/PickUpItems/UsableItems/FireWeapon/JT_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameObjects/JT_DamageInfoActor.h"

AJT_Projectile::AJT_Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AJT_Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 15000.f;
	ProjectileMovement->MaxSpeed = 15000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 2 seconds by default
	InitialLifeSpan = 2.0f;
}

void AJT_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float TakedDamage = OtherActor->TakeDamage(Damage, FDamageEvent(), nullptr, this);
	// Only add impulse if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseCoef, GetActorLocation());
	}

	//TODO Move this logic to DamageInfo actor component
	/*Show damage*/
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	FTransform Transform = FTransform(GetActorRotation(), Hit.Location, FVector(1.0f, 1.0f, 1.0f));
	AJT_DamageInfoActor* DamageActor = GetWorld()->SpawnActor<AJT_DamageInfoActor>(DamageInfoActorClass, Transform);
	DamageActor->SetDamageText(FText::FromString(FString::SanitizeFloat(TakedDamage)));

	Destroy();
}

