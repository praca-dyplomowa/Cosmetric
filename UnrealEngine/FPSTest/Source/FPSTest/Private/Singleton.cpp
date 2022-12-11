// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton.h"
#include "../FPSTestCharacter.h"
#include <cmath>
#include <random>
#include <algorithm>
#include "../Public/PerlinNoise.h"
#include "GUI/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASingleton::ASingleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingleton::BeginPlay()
{
	Super::BeginPlay();
	int saveGameIndex = 0;
	FString saveGameName = TEXT("New Game Save");

	LoadGameFromMemory(saveGameIndex, saveGameName);
}

// Called every frame
void ASingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASingleton::SaveGameToMemory(int saveSlotIndex, FString saveSlotName)
{
	GameInfo->PlayerTransform = FTransform(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetActorTransform());
	return UGameplayStatics::SaveGameToSlot(GameInfo, saveSlotName, saveSlotIndex);
}

bool ASingleton::LoadGameFromMemory(int saveSlotIndex, FString saveSlotName)
{

	if(!UGameplayStatics::DoesSaveGameExist(saveSlotName, saveSlotIndex)) {
		GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::CreateSaveGameObject(UGameSaveInfo::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));

		return true;
	}

	GameInfo = Cast<UGameSaveInfo>(UGameplayStatics::LoadGameFromSlot(saveSlotName, saveSlotIndex));
	if (! GameInfo) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
		return false;
	}

	GameInfo->UserIndex = saveSlotIndex;
	GameInfo->SaveSlotName = saveSlotName;
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->SetActorTransform(GameInfo->PlayerTransform);
	
	return true;
	
}

