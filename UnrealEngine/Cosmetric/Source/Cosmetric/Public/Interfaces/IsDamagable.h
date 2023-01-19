// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IsScannable.h"
#include "../FPSTestCharacter.h"
#include "IsDamagable.generated.h"

/**
 * 
 */
UCLASS()
class COSMETRIC_API AIsDamagable : public AIsScannable
{
	GENERATED_BODY()

public:
	AIsDamagable();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	virtual void OnKilled();
	virtual void OnDamageTaken();

public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	float Health;
	bool Alive;
	bool BeingDamaged;
	float DamageTime;
	UFUNCTION()
	void OnSelectedDamage(AActor* Target, FKey ButtonPressed);


};
