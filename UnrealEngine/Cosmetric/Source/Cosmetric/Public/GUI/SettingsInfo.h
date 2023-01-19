// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsInfo.generated.h"

/**
 * 
 */
UCLASS()
class COSMETRIC_API USettingsInfo : public USaveGame
{
	GENERATED_BODY()

public:

	USettingsInfo();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		FString Resolution;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		bool Windowed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int Shadows;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int RenderDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int Textures;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int ResolutionScaling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int AntiAliasing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Graphics)
		int PostProcessing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool Tutorials;
	
};
