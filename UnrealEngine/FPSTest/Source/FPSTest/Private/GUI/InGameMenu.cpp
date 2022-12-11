// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/InGameMenu.h"
#include "Components/Button.h"
#include "../FPSTestCharacter.h"

void UInGameMenu::NativeConstruct()
{
	Super::NativeConstruct();
	ContinueButton->OnClicked.AddUniqueDynamic(this, &UInGameMenu::OnContinueButtonClicked);
}

void UInGameMenu::OnContinueButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->SetPause(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}
