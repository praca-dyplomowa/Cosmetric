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
	cubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Load the Cube mesh
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	UMaterial* mt = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Ground_Grass"));
	// Set the component's mesh
	cubeMeshComponent->SetStaticMesh(cubeMesh);
	cubeMeshComponent->SetMaterial(0, mt);
	OnClicked.AddUniqueDynamic(this, &AIsScannable::OnSelected);
	OnReleased.AddUniqueDynamic(this, &AIsScannable::OnUnselected);
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
		if (Time <= 0)
		{
			BeingScanned = false;
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
			ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
			((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Add(Name);
		}
		ScanProgressBar->SetValue(Time, 5.0);
		Time -= DeltaTime;
	}
}

void AIsScannable::OnSelected(AActor* Target, FKey ButtonPressed)
{
	if (!(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Catalog.Contains(Name)))
	{
		((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = false;
		ScanProgressBar->SetVisibility(ESlateVisibility::Visible);
		BeingScanned = true;
		Time = 5.0;
	}
}

void AIsScannable::OnUnselected(AActor* Target, FKey ButtonPressed)
{
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
	ScanProgressBar->SetVisibility(ESlateVisibility::Hidden);
	BeingScanned = false;
}