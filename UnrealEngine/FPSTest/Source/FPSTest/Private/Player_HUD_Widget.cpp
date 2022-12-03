// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_HUD_Widget.h"

void UPlayer_HUD_Widget::SetHealth(float val)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(val / 100.0);
	}
}

void UPlayer_HUD_Widget::SetHunger(float val)
{
	if (HungerBar)
	{
		HungerBar->SetPercent(val / 100.0);
	}
}

void UPlayer_HUD_Widget::SetTemp(float val)
{
	if (Temperature)
	{
		Temperature->SetText(FText::FromString(FString::SanitizeFloat(val)));
	}
}

void UPlayer_HUD_Widget::SetFood(float val)
{
	if (FoodBar)
	{
		FoodBar->SetPercent(val / 100.0);
	}
}
