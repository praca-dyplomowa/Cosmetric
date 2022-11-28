// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ProgressBar.h"
#include "IsScannable.generated.h"

class UProgressBar;

UCLASS()
class FPSTEST_API AIsScannable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsScannable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* cubeMeshComponent;

	double time;

	UFUNCTION()
		void OnSelected(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnselected(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnhovered(AActor* Target);
};
