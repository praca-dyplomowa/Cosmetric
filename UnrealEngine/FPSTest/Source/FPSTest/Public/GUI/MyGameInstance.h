// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameSaveInfo.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGameSaveInfo* GameInfo;

	UFUNCTION(BlueprintCallable)
	bool SaveGameToMemory(FString saveSlotName, int saveUserIndex);
	UFUNCTION(BlueprintCallable)
	bool LoadGameFromMemory(FString saveSlotName, int saveUserIndex);

	UFUNCTION(BlueprintCallable)
		void SetSeed(int seed);

	UFUNCTION(BlueprintCallable)
		bool CreateNewSaveInstance(FString saveSlotName, int saveUserIndex);
	
};
