// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjects/JT_InteractiveObjectInterface.h"
#include "JT_InteractiveObjectBase.generated.h"

UCLASS(Abstract)
class JOYTEST_API AJT_InteractiveObjectBase : public AActor, public IJT_InteractiveObjectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJT_InteractiveObjectBase();



	//c++ public methods
public:

	virtual void BeginPlay() override;



	//>>............................................................................................................<<//


	//Blueprint methods
public:

	virtual void OnActivatedForInteract_Implementation(AActor* InstigatedBy) {};
	virtual FText GetInteractiveMessage_Implementation();
	//Blueprint values
public:

	FText InteractionMessage = FText::FromString("Interact");

};
