// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "Kismet/GameplayStatics.h"
#include "Singleton.h"

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
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleton::StaticClass(), ActorsToFind);
	for (AActor* Singleton : ActorsToFind)
	{
		ASingleton* single = Cast<ASingleton>(Singleton);
		if (single)
		{
			FBuildingInfo buildingInfo;
			buildingInfo.Transform = GetActorTransform();
			buildingInfo.Class = GetClass();
			if (single->GameInfo != nullptr) {
				single->GameInfo->BuildingInfo.Add(buildingInfo);
			}
			
		}
	}
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

