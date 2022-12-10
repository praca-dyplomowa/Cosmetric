// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveInfo.generated.h"

USTRUCT(BlueprintType)
struct FChunkSaveInfo {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSet<FVector2D> DestroyedTreePositions;
};

/**
 * 
 */
UCLASS()
class FPSTEST_API UGameSaveInfo : public USaveGame
{
	UGameSaveInfo();
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FTransform PlayerTransform;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TMap<FVector, FChunkSaveInfo> ChunkInfo;
};
