// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IsCollectable.h"
#include "HUD/AllPurposeProgressBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "AnimalBase.generated.h"

UCLASS()
class FPSTEST_API AAnimalBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimalBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Values
	//shape
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int FlagHead = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int FlagBody = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int FlagLegs = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int FlagTail = 1;

	//size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float ScaleAnimal = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float ScaleHead = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float ScaleBody = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float ScaleLegs = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		float ScaleTail = 1.0;

	//color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		int Blue;

public:
	//AIsActionable
	bool Action;
	//AIsCollectable
	virtual void OnCollected();
	float CollectTime;
	bool BeingCollected;
	bool Collectable;
	static const inline float MaxCollectTime = 3.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boop")
		float Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop")
		float Material;
	UAllPurposeProgressBarWidget* CollectProgressBar;

	UFUNCTION()
		void OnSelectedCollect(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnselectedCollect(AActor* Target, FKey ButtonPressed);
	//AIsDamagable
	UFUNCTION()
		virtual void OnKilled();
	virtual void OnDamageTaken();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop")
		float Health;
	bool Alive;
	bool BeingDamaged;
	float DamageTime;
	UFUNCTION()
		void OnSelectedDamage(AActor* Target, FKey ButtonPressed);
	//AIsScannable
	float ScanTime;
	bool BeingScanned;
	static const inline float MaxScanTime = 5.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop")
		FString Name;

	TSubclassOf<class UAllPurposeProgressBarWidget> ProgressBarWidgetClass;

	UPROPERTY()
		class UAllPurposeProgressBarWidget* ScanProgressBar;

	UFUNCTION()
		void OnSelectedScan(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
		void OnUnselectedScan(AActor* Target, FKey ButtonPressed);
};
