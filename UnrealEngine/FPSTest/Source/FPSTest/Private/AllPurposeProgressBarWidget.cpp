// Fill out your copyright notice in the Description page of Project Settings.


#include "AllPurposeProgressBarWidget.h"

void UAllPurposeProgressBarWidget::SetValue(float val, float max)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(val / max);
	}
}

void UAllPurposeProgressBarWidget::SetText(FString txt)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(txt));
	}
}