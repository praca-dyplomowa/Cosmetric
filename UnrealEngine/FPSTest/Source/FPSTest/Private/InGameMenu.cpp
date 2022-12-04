// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "../FPSTestCharacter.h"

void UInGameMenu::NativeConstruct()
{
	Super::NativeConstruct();
	ContinueButton->OnClicked.AddUniqueDynamic(this, &UInGameMenu::OnContinueButtonClicked);
}

void UInGameMenu::OnContinueButtonClicked()
{
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->menuing = false;
	this->SetVisibility(ESlateVisibility::Hidden);
}
