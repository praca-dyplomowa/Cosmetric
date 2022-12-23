// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalBase.h"
#include "Components/ProgressBar.h"
#include "../FPSTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Singleton.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values
AAnimalBase::AAnimalBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Action = false;
	OnClicked.AddUniqueDynamic(this, &AAnimalBase::OnSelectedScan);
	OnReleased.AddUniqueDynamic(this, &AAnimalBase::OnUnselectedScan);
	// OnEndCursorOver.AddUniqueDynamic(this, &AAnimalBase::OnEndCursorOverScan);
	ScanProgressBar = nullptr;
	BeingScanned = false;
	ProgressBarWidgetClass = nullptr;
	OnClicked.AddUniqueDynamic(this, &AAnimalBase::OnSelectedDamage);
	Alive = true;
	BeingDamaged = false;
	OnClicked.AddUniqueDynamic(this, &AAnimalBase::OnSelectedCollect);
	OnReleased.AddUniqueDynamic(this, &AAnimalBase::OnUnselectedCollect);
	// OnEndCursorOver.AddUniqueDynamic(this, &AAnimalBase::OnEndCursorOverCollect);
	CollectProgressBar = nullptr;
	BeingCollected = false;
	Collectable = true;
}

// Called when the game starts or when spawned
void AAnimalBase::BeginPlay()
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

void AAnimalBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CollectProgressBar)
	{
		CollectProgressBar->RemoveFromParent();
		CollectProgressBar = nullptr;
	}
	if (ScanProgressBar)
	{
		ScanProgressBar->RemoveFromParent();
		ScanProgressBar = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AAnimalBase::Tick(float DeltaTime)
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
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->AnimalCatalog.Add(Name, spcNum);
			Action = false;
		}
		ScanProgressBar->SetValue(ScanTime, MaxScanTime);
		ScanTime -= DeltaTime;
	}
	if (BeingDamaged)
	{
		if (DamageTime <= 0)
		{
			BeingDamaged = false;
			Action = false;
		}
		DamageTime -= DeltaTime;
	}
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
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Food += Food;
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->AnimalMaterial += Material;
			Action = false;
			this->SetLifeSpan(5);
		}
		CollectProgressBar->SetValue(CollectTime, MaxCollectTime);
		CollectTime -= DeltaTime;
		if (!Collectable) {
			OnCollected();
		}
	}
}

// Called to bind functionality to input
void AAnimalBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAnimalBase::OnSelectedScan(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::RightMouseButton == ButtonPressed && !Action)
	{
		if (500 >= this->GetDistanceTo(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))))
		{
			if (!(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->AnimalCatalog.Contains(Name)))
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

void AAnimalBase::OnUnselectedScan(AActor* Target, FKey ButtonPressed)
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

void AAnimalBase::OnEndCursorOverScan(AActor* Target)
{
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
	if (ScanProgressBar)
	{
		ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	BeingScanned = false;
	Action = false;
}

void AAnimalBase::OnSelectedDamage(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::LeftMouseButton == ButtonPressed)
	{
		if (500 >= this->GetDistanceTo(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))))
		{
			if (Alive && !BeingDamaged)
			{
				OnDamageTaken();
				Health -= 1.0;
				if (Health > 0.0)
				{
					BeingDamaged = true;
					DamageTime = 1.0;
					Action = true;
				}
				else
				{
					BeingDamaged = true;
					DamageTime = 0.1;
					Action = true;
					OnKilled();
				}
			}
		}
	}
}

void AAnimalBase::OnSelectedCollect(AActor* Target, FKey ButtonPressed)
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

void AAnimalBase::OnUnselectedCollect(AActor* Target, FKey ButtonPressed)
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

void AAnimalBase::OnEndCursorOverCollect(AActor* Target)
{
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
	if (CollectProgressBar)
	{
		CollectProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	BeingCollected = false;
	Action = false;
}

void AAnimalBase::OnKilled()
{
	Alive = false;
}

void AAnimalBase::OnDamageTaken()
{
	FVector loc(0.0, 0.0, 100.0);
	AddActorLocalOffset(loc);
}

void AAnimalBase::OnCollected() { }