// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IsScannable.h"
#include "AllPurposeProgressBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "IsCollectable.generated.h"

UCLASS()
class FPSTEST_API AIsCollectable : public AIsScannable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIsCollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float CollectTime;
	bool BeingCollected;
	bool Collectable;
	UPROPERTY(EditAnywhere)
		int Materials[3];

		TSubclassOf<class UAllPurposeProgressBarWidget> CollectProgressBarWidgetClass;

	UPROPERTY()
		class UAllPurposeProgressBarWidget* CollectProgressBar;

	UFUNCTION()
		void OnSelectedCollect(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnselectedCollect(AActor* Target, FKey ButtonPressed);
};