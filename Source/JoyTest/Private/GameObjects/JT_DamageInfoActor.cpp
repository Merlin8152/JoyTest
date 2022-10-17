// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/JT_DamageInfoActor.h"
#include "Components/WidgetComponent.h"
#include "Widgets/JT_DamageInfoWD.h"
// Sets default values
AJT_DamageInfoActor::AJT_DamageInfoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);


}

// Called when the game starts or when spawned
void AJT_DamageInfoActor::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("OnDestroyTimer"));
	GetWorldTimerManager().SetTimer(DestroyTimer, TimerDel, TimeToDestroy, false);
	WidgetComponent->SetWidgetClass(WidgetClass);
	WidgetComponent->UpdateWidget();
}

void AJT_DamageInfoActor::OnDestroyTimer()
{
	Destroy();
}

void AJT_DamageInfoActor::SetDamageText(const FText& Text)
{
	if (UJT_DamageInfoWD* DamageInfoWD = Cast<UJT_DamageInfoWD>(WidgetComponent->GetWidget()))
	{
		DamageInfoWD->SetDamageText(Text);
	}
}

// Called every frame
void AJT_DamageInfoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

