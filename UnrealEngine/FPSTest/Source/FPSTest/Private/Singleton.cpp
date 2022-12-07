// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton.h"
#include "../FPSTestCharacter.h"
#include "../Public/MyGameInstance.h"

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
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->menuing = false;
	this->Seed = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->Seed;
}

// Called every frame
void ASingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

