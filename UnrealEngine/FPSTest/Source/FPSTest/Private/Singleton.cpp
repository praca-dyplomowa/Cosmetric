// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton.h"
#include "../FPSTestCharacter.h"
#include <cmath>
#include <random>
#include <algorithm>
#include "BuildingBase.h"
#include "../Public/PerlinNoise.h"
#include "GUI/MyGameInstance.h"

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
	auto character = ((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	if (GameInfo) {
		FString stats = GameInfo->PlayerStats.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Loaded Player Stats\n%s\nfrom save slot %s"), *stats, *GameInfo->SaveSlotName);
		character->SetActorTransform(GameInfo->PlayerTransform);
		character->SetStats(GameInfo->PlayerStats);
		for (auto& Building : GameInfo->BuildingInfo)
		{
			GetWorld()->SpawnActor<ABuildingBase>(Building.Class, Building.Transform);
		}
	}
}

// Called every frame
void ASingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

