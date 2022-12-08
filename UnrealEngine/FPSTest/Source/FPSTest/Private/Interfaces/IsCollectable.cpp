// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IsCollectable.h"
#include "Components/ProgressBar.h"
#include "../FPSTestCharacter.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
AIsCollectable::AIsCollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnClicked.AddUniqueDynamic(this, &AIsCollectable::OnSelectedCollect);
	OnReleased.AddUniqueDynamic(this, &AIsCollectable::OnUnselectedCollect);
	CollectProgressBar = nullptr;
	BeingCollected = false;
	Collectable = true;
}

// Called when the game starts or when spawned
void AIsCollectable::BeginPlay()
{
	Super::BeginPlay();
	if (ProgressBarWidgetClass)
	{
		APlayerController* ctr = GetWorld()->GetFirstPlayerController();
		check(ctr);
		CollectProgressBar = CreateWidget<UAllPurposeProgressBarWidget>(ctr, ProgressBarWidgetClass);
		check(CollectProgressBar);
		CollectProgressBar->AddToPlayerScreen();
		CollectProgressBar->SetText("Collecting");
		CollectProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AIsCollectable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CollectProgressBar)
	{
		CollectProgressBar->RemoveFromParent();
		CollectProgressBar = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AIsCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BeingCollected)
	{
		if (CollectTime <= 0)
		{
			BeingCollected = false;
			Collectable = false;
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
			if (CollectProgressBar)
			{
				CollectProgressBar->SetVisibility(ESlateVisibility::Hidden);
			}
			for (int i = 0; i < 3; i++)
				((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Equipment[i] += Materials[i];
			Action = false;
		}
		CollectProgressBar->SetValue(CollectTime, MaxCollectTime);
		CollectTime -= DeltaTime;
	}
}

void AIsCollectable::OnSelectedCollect(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::LeftMouseButton == ButtonPressed && !Action && !Alive)
	{
		if (Collectable)
		{
			if (500 >= this->GetDistanceTo(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))))
			{
				((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = false;
				if (CollectProgressBar)
				{
					CollectProgressBar->SetVisibility(ESlateVisibility::Visible);
				}
				BeingCollected = true;
				CollectTime = MaxCollectTime;
				Action = true;
			}
		}
	}
}

void AIsCollectable::OnUnselectedCollect(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::LeftMouseButton == ButtonPressed && BeingCollected)
	{
		((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
		if (CollectProgressBar)
		{
			CollectProgressBar->SetVisibility(ESlateVisibility::Hidden);
		}
		BeingCollected = false;
		Action = false;
	}
}