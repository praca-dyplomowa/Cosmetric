// Fill out your copyright notice in the Description page of Project Settings.


#include "IsScannable.h"
#include "Components/ProgressBar.h"
#include "../FPSTestCharacter.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
AIsScannable::AIsScannable()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnClicked.AddUniqueDynamic(this, &AIsScannable::OnSelectedScan);
	OnReleased.AddUniqueDynamic(this, &AIsScannable::OnUnselectedScan);
	ProgressBarWidgetClass = nullptr;
	ScanProgressBar = nullptr;
	BeingScanned = false;
}

// Called when the game starts or when spawned
void AIsScannable::BeginPlay()
{
	Super::BeginPlay();
	if (ProgressBarWidgetClass)
	{
		APlayerController* ctr = GetWorld()->GetFirstPlayerController();
		check(ctr);
		ScanProgressBar = CreateWidget<UAllPurposeProgressBarWidget>(ctr, ProgressBarWidgetClass);
		check(ScanProgressBar);
		ScanProgressBar->AddToPlayerScreen();
		ScanProgressBar->SetText("Scanning");
		ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AIsScannable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ScanProgressBar)
	{
		ScanProgressBar->RemoveFromParent();
		ScanProgressBar = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AIsScannable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BeingScanned)
	{
		if (ScanTime <= 0)
		{
			BeingScanned = false;
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
			ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Add(Name);
		}
		ScanProgressBar->SetValue(ScanTime, 5.0);
		ScanTime -= DeltaTime;
	}
}

void AIsScannable::OnSelectedScan(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::RightMouseButton == ButtonPressed)
	{
		if (!(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Contains(Name)))
		{
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = false;
			ScanProgressBar->SetVisibility(ESlateVisibility::Visible);
			BeingScanned = true;
			ScanTime = 5.0;
		}
	}
}

void AIsScannable::OnUnselectedScan(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::RightMouseButton == ButtonPressed)
	{
		((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
		ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
		BeingScanned = false;
	}
}