// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IsScannable.h"
#include "Components/ProgressBar.h"
#include "../FPSTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Singleton.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIsScannable::AIsScannable()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnClicked.AddUniqueDynamic(this, &AIsScannable::OnSelectedScan);
	OnReleased.AddUniqueDynamic(this, &AIsScannable::OnUnselectedScan);
	ScanProgressBar = nullptr;
	BeingScanned = false;
	ProgressBarWidgetClass = nullptr;
}

// Called when the game starts or when spawned
void AIsScannable::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleton::StaticClass(), ActorsToFind);
	for (AActor* Singleton : ActorsToFind)
	{
		ASingleton* single = Cast<ASingleton>(Singleton);
		if (single)
		{
			ProgressBarWidgetClass = single->ProgressBarWidgetClass;
		}
	}
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
			if (ScanProgressBar)
			{
				ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
			}
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Add(Name);
			Action = false;
		}
		ScanProgressBar->SetValue(ScanTime, MaxScanTime);
		ScanTime -= DeltaTime;
	}
}

void AIsScannable::OnSelectedScan(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::RightMouseButton == ButtonPressed && !Action)
	{
		if (500 >= this->GetDistanceTo(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))))
		{
			if (!(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Contains(Name)))
			{
				((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = false;
				if (ScanProgressBar)
				{
					ScanProgressBar->SetVisibility(ESlateVisibility::Visible);
				}
				BeingScanned = true;
				ScanTime = MaxScanTime;
				Action = true;
			}
		}
	}
}

void AIsScannable::OnUnselectedScan(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::RightMouseButton == ButtonPressed && BeingScanned)
	{
		((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
		if (ScanProgressBar)
		{
			ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
		}
		BeingScanned = false;
		Action = false;
	}
}