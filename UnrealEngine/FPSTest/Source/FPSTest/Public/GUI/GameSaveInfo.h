// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../FPSTestCharacter.h"
#include "BuildingBase.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveInfo.generated.h"

FORCEINLINE bool operator==(const FTransform& a, const FTransform& b)
{
	return a.Equals(b, 0.01);
}

USTRUCT(BlueprintType)
struct FChunkSaveInfo {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSet<FVector2D> DestroyedTreePositions;
};

USTRUCT(BlueprintType)
struct FBuildingInfo {
	GENERATED_BODY()
		FBuildingInfo(const FTransform& transform, TSubclassOf<ABuildingBase> myClass);

	FBuildingInfo();
	
	UPROPERTY(VisibleAnywhere)
		FTransform Transform;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<ABuildingBase> Class;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		int Seed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		int UserIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
		FTransform PlayerTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	FCompactPlayerStats PlayerStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Chunks)
		TMap<FVector, FChunkSaveInfo> ChunkInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Buildings)
		TArray<FBuildingInfo> BuildingInfo;

};
