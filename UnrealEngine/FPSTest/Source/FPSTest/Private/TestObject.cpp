// Fill out your copyright notice in the Description page of Project Settings.


#include "TestObject.h"

// Sets default values
ATestObject::ATestObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	AActor::PrimaryActorTick.bCanEverTick = true;
	//auto cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'")).Object;
	//auto cubeMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	//Cube = NewObject<UStaticMeshComponent>(this, TEXT("Root"));
	//SetRootComponent(Cube);
	//Cube->SetStaticMesh(cubeMesh);
	//Cube->SetMaterial(0, cubeMaterial);
}

// Called when the game starts or when spawned
void ATestObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

