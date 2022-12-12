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
	int saveGameIndex = 0;
	FString saveGameName = TEXT("New Game Save");
	this->GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	
	auto character = ((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));
	if (GameInfo) {
		character->SetActorTransform(GameInfo->PlayerTransform);
		character->SetStats(GameInfo->PlayerStats);
	}

}

// Called every frame
void ASingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

