// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Player_HUD_Widget.h"

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
		Temperature->SetText(FText::FromString(FString::FromInt((int)floor(val))));
	}
	if (TemperatureBar)
	{
		TemperatureBar->SetPercent((val + 25.0) / 70.0);
		TemperatureBar->FillColorAndOpacity.A = 1.0;
		if (val < 0)
		{
			TemperatureBar->FillColorAndOpacity.R = 0.0;
			TemperatureBar->FillColorAndOpacity.G = 0.5;
			TemperatureBar->FillColorAndOpacity.B = 1.0;
		}
		else
		{
			TemperatureBar->FillColorAndOpacity.R = 1.0;
			TemperatureBar->FillColorAndOpacity.G = 0.0;
			TemperatureBar->FillColorAndOpacity.B = 0.0;
		}
	}
}

void UPlayer_HUD_Widget::SetFood(float val)
{
	if (FoodBar)
	{
		FoodBar->SetPercent(val / 100.0);
	}
}
