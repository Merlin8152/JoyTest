// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjects/JT_InteractiveObjectBase.h"

// Sets default values
AJT_InteractiveObjectBase::AJT_InteractiveObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AJT_InteractiveObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

FText AJT_InteractiveObjectBase::GetInteractiveMessage_Implementation()
{
	return InteractionMessage;
}


