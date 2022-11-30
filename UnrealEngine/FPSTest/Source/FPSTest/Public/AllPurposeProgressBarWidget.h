// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "AllPurposeProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API UAllPurposeProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetValue(float val, float max);

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ProgressBar;
	
};
