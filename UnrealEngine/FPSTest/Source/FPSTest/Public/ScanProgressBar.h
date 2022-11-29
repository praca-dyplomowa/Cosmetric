// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScanProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API UScanProgressBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
};
