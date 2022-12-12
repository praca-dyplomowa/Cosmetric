// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UMyGameInstance::SaveGameToMemory(FString saveSlotName, int saveUserIndex)
{
	auto character = ((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));
	GameInfo->PlayerTransform = FTransform(character->GetActorTransform());
	GameInfo->PlayerStats = character->GetStats();
	return UGameplayStatics::SaveGameToSlot(GameInfo, saveSlotName, saveUserIndex);
}

bool UMyGameInstance::LoadGameFromMemory(FString saveSlotName, int saveUserIndex)
{
	GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::LoadGameFromSlot(saveSlotName, saveUserIndex));
	if (!GameInfo) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded  SaveGame Data with name %s"), &saveSlotName);

	GameInfo->UserIndex = saveUserIndex;
	GameInfo->SaveSlotName = saveSlotName;

	for (auto& Building : GameInfo->BuildingInfo)
	{
		GetWorld()->SpawnActor<ABuildingBase>(Building.Class, Building.Transform);
	}

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
	if (!UGameplayStatics::DoesSaveGameExist(saveSlotName, saveUserIndex)) {
		GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::CreateSaveGameObject(UGameSaveInfo::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data with name %s"), &saveSlotName);
		// set initial player stats
		GameInfo->PlayerTransform = FTransform(
			FRotator(),
			FVector(0, 0, 100),
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

		return true;
	}
	return false;
}
