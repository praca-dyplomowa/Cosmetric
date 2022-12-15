// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalMenager.h"

// Sets default values
AAnimalMenager::AAnimalMenager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnimalMenager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimalMenager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnimalMenager::init(int Seed)
{
	for (int i = 0; i < 50; i++)
	{
		auto Stream = FRandomStream(Seed + i * i);
		FSpecies spc;
		spc.bodyflag = 1 + floor(Stream.GetFraction() * _bodytypes);
		spc.headflag = 1 + floor(Stream.GetFraction() * _headtypes);
		spc.tailflag = 1 + floor(Stream.GetFraction() * _tailtypes);
		spc.legsflag = 1 + floor(Stream.GetFraction() * _legstypes);
		spc.red = floor(Stream.GetFraction() * 256);
		spc.green = floor(Stream.GetFraction() * 256);
		spc.blue = floor(Stream.GetFraction() * 256);
		spc.animalscale = 0.5 + Stream.GetFraction();
		spc.bodyscale = 1.0 + Stream.GetFraction();
		spc.headscale = 1.0 + Stream.GetFraction();
		spc.tailscale = 1.0 + Stream.GetFraction();
		spc.legsscale = 1.0 + Stream.GetFraction();
		Species.Add(spc);
	}
}

