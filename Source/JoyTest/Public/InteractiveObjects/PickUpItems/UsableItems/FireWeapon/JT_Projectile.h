// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JT_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AJT_DamageInfoActor;

UCLASS(Abstract)
class JOYTEST_API AJT_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJT_Projectile();

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

protected:


	float Damage = 10.0f;

	/*
	* Impulse to phys targets = (Velocity * ImpulseCoef)
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Setup")
		float ImpulseCoef = 100.0f;

	/*
	* DamageInfoActor class to spawn on hit
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Setup")
		TSubclassOf<AJT_DamageInfoActor> DamageInfoActorClass = nullptr;
public:


	UFUNCTION(BlueprintCallable)
		void SetDamage(float InDamage) { Damage = InDamage; };
	UFUNCTION(BlueprintCallable)
		float GetDamage() { return Damage; };


	/** called when projectile hits something */
	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

};
