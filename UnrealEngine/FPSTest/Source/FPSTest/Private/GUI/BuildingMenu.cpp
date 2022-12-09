// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/BuildingMenu.h"
#include "Components/Button.h"
#include "../FPSTestCharacter.h"

void UBuildingMenu::NativeConstruct()
{
	Super::NativeConstruct();
	ContinueButton->OnClicked.AddUniqueDynamic(this, &UBuildingMenu::OnContinueButtonClicked);
}

void UBuildingMenu::OnContinueButtonClicked()
{
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->menuing = false;
	this->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->SetPause(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}

void UBuildingMenu::SetEQ()
{
	AFPSTestCharacter* tmp = ((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));
	EQ->SetText(FText::FromString("Drewno"+FString::SanitizeFloat(tmp->Wood)+"\nSkora:"+ FString::SanitizeFloat(tmp->AnimalMaterial)));
}