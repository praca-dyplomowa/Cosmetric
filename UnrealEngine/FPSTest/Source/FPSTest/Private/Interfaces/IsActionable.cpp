// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IsActionable.h"

// Sets default values
AIsActionable::AIsActionable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Action = false;
}

// Called when the game starts or when spawned
void AIsActionable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsActionable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

