// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AllPurposeProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API UAllPurposeProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetValue(float val, float max);
	UFUNCTION(BlueprintCallable)
	void SetText(FString txt);

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ProgressBar;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TextBlock;
	
};
