// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IsActionable.h"
#include "HUD/AllPurposeProgressBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "IsScannable.generated.h"

UCLASS()
class COSMETRIC_API AIsScannable : public AIsActionable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsScannable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float ScanTime;
	bool BeingScanned;
	static const inline float MaxScanTime = 5.0;
	UPROPERTY(EditAnywhere)
		FString Name;

	TSubclassOf<class UAllPurposeProgressBarWidget> ProgressBarWidgetClass;

	UPROPERTY()
		class UAllPurposeProgressBarWidget* ScanProgressBar;

	UFUNCTION()
		void OnSelectedScan(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnselectedScan(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnEndCursorOverScan(AActor* Target);
};
