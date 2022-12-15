// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "Kismet/GameplayStatics.h"
#include "GUI/MyGameInstance.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildingBase::RegisterToSaveInfo()
{
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	if (GameInfo)
	{
		FBuildingInfo buildingInfo;
		buildingInfo.Transform = GetActorTransform();
		buildingInfo.Class = GetClass();
		GameInfo->BuildingInfo.Add(buildingInfo);
	}
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

