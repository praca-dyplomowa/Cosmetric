// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UMyGameInstance::SaveGameToMemory(FString saveSlotName, int saveUserIndex)
{
	auto character = ((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));
	GameInfo->PlayerTransform = FTransform(character->GetActorTransform());
	GameInfo->PlayerStats = character->GetStats();
	UE_LOG(LogTemp, Warning, TEXT("Saved SaveGame Data with name %s"), *saveSlotName);
	auto stats = GameInfo->PlayerStats.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Saved Player Stats\n%s"), *stats);
	return UGameplayStatics::SaveGameToSlot(GameInfo, saveSlotName, saveUserIndex);
}

bool UMyGameInstance::LoadGameFromMemory(FString saveSlotName, int saveUserIndex)
{
	GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::LoadGameFromSlot(saveSlotName, saveUserIndex));
	if (!GameInfo) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data with name %s"), *saveSlotName);
	auto stats = GameInfo->PlayerStats.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Loaded Player Stats\n%s"), *stats);
	GameInfo->UserIndex = saveUserIndex;
	GameInfo->SaveSlotName = saveSlotName;

	return true;

}

void UMyGameInstance::SetSeed(int seed)
{
	if (GameInfo) {
		GameInfo->Seed = seed;
	}
}

bool UMyGameInstance::CreateNewSaveInstance(FString saveSlotName, int saveUserIndex)
{
	GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::CreateSaveGameObject(UGameSaveInfo::StaticClass()));
	if (GameInfo == nullptr) return false;
	UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data with name %s"), *saveSlotName);
	// set initial player stats
	GameInfo->PlayerTransform = FTransform(
		FRotator(0, 0, 0),
		FVector(610, 1520, 280),
		FVector(1)
	);

	FCompactPlayerStats stats;
	stats.Health = 100.0;
	stats.Hunger = 100.0;
	stats.Food = 100.0;
	stats.Wood = 100.0;
	stats.AnimalMaterial = 100.0;
	stats.Temperature = 0.0;
	GameInfo->PlayerStats = stats;
	GameInfo->SaveSlotName = saveSlotName;
	GameInfo->UserIndex = saveUserIndex;
	return true;
}
