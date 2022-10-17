// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JT_DamageInfoWD.h"
#include "Components/TextBlock.h"

void UJT_DamageInfoWD::SetDamageText(const FText& Text)
{
	if (DamageText) DamageText->SetText(Text);
}
