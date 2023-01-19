// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class COSMETRIC_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;

	UFUNCTION()
		void OnContinueButtonClicked();
};
