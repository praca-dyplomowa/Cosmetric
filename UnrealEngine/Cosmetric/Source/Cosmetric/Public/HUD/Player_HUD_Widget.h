// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player_HUD_Widget.generated.h"

/**
 * 
 */
UCLASS()
class COSMETRIC_API UPlayer_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealth(float val);
	void SetHunger(float val);
	void SetTemp(float val);
	void SetFood(float val);

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HungerBar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* FoodBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Temperature;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* TemperatureBar;
};
