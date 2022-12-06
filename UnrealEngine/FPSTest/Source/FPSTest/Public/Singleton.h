// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "Singleton.generated.h"

UCLASS()
class FPSTEST_API ASingleton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UAllPurposeProgressBarWidget> ProgressBarWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPlayer_HUD_Widget> HUDClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)

		TSubclassOf<class UMyGameInstance> GameInstanceClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UInGameMenu> PauseClass;
};
