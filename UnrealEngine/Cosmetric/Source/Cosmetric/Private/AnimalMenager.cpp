// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalMenager.h"
#include "../Public/Singleton.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/GUI/MyGameInstance.h"

TArray<FString> FirstName = {
	FString("Equus"),
	FString("Canis"),
	FString("Ovis"),
	FString("Bos"),
	FString("Felis"),
	FString("Ursus"),
	FString("Loxodenta"),
	FString("Panthera"),
	FString("Giraffa"),
	FString("Mesocricetus"),
	FString("Phodopus"),
	FString("Cricetulus"),
	FString("Tscherskia"),
	FString("Cansumys"),
	FString("Allocricetulus"),
};

TArray<FString> SecondName = {
	FString("ferus"),
	FString("hemious"),
	FString("kiang"),
	FString("quagga"),
	FString("aries"),
	FString("ammon"),
	FString("canadnsis"),
	FString("dalli"),
	FString("gmelini"),
	FString("nivicola"),
	FString("vignei"),
	FString("taurus"),
	FString("frontalis"),
	FString("gaurus"),
	FString("grunniens"),
	FString("indicus"),
	FString("javanicus"),
	FString("mutus"),
	FString("sauveli"),
	FString("bonasus"),
	FString("bison"),
	FString("ursinus"),
	FString("arctos"),
	FString("maritimus"),
	FString("cyclotis"),
	FString("elephas"),
	FString("catus"),
	FString("chaus"),
	FString("silvesrtis"),
	FString("lybica"),
};

TArray<FString> ThirdName = {
	FString(""),
	FString("minor"),
	FString("major"),
	FString("magnum"),
	FString("mediocriter"),
	FString("parvus"),
	FString("stultus"),
	FString("escam"),
	FString("wiry"),
	FString("potens"),
	FString("infirma"),
	FString("mercurii"),
	FString("lunae"),
	FString("martis"),
	FString("lovis"),
	FString("veneris"),
	FString("sabbatum"),
	FString("solis"),
	FString("recentibus"),
	FString("circum"),
};

// Sets default values
AAnimalMenager::AAnimalMenager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	distribution = std::uniform_real_distribution<float>(0.0, 1.0);
}

// Called when the game starts or when spawned
void AAnimalMenager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleton::StaticClass(), ActorsToFind);
	for (AActor* Singleton : ActorsToFind)
	{
		ASingleton* single = Cast<ASingleton>(Singleton);
		if (single)
		{
			AnimalClass = single->AnimalClass;
		}
	}
	auto GameInstance = ((UMyGameInstance*)(GetWorld()->GetGameInstance()));
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	if (GameInfo) {
		Seed = GameInfo->Seed;
	}
	if (GameInstance) {
		RenderDistance = GameInstance->RenderDistance;
	}
	for (int i = 0; i < 256; i++)
	{
		perm[i] = i;
	}
	std::shuffle(&perm[0], &perm[255], std::default_random_engine(Seed));
}

// Called every frame
void AAnimalMenager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimalSpawnTimer -= DeltaTime;
	if (0.0 >= AnimalSpawnTimer)
	{
		TArray<AActor*> ActorsToFind;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnimalBase::StaticClass(), ActorsToFind);
		FVector loc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		for (int i = ActorsToFind.Num(); i < 2 * RenderDistance * RenderDistance; i++)
		{
			float x = distribution(generator);
			float fx = (x - 0.5) * RenderDistance * 2000;
			x += x - 0.5 < 0.0 ? loc.X + fx - 500 : loc.X + fx + 500;
			float y = distribution(generator);
			float fy = (y - 0.5) * RenderDistance * 2000;
			y += y - 0.5 < 0.0 ? loc.Y + fy - 500 : loc.Y + fy + 500;
			FTransform pos = FTransform(FVector(
				x,
				y,
				PerlinNoise::Noise(x / 2000, y / 2000, perm) + 100.0)
			);
			AAnimalBase* ani = GetWorld()->SpawnActor<AAnimalBase>(AnimalClass, pos);
		}
		AnimalSpawnTimer = 60.0;
	}
}

void AAnimalMenager::init(int seed)
{
	for (int i = 0; i < 50; i++)
	{
		auto Stream = FRandomStream(seed + i * i);
		FSpecies spc;
		spc.bodyflag = 1 + floor(Stream.GetFraction() * 5);
		spc.headflag = 1 + floor(Stream.GetFraction() * 5);
		spc.tailflag = 1 + floor(Stream.GetFraction() * 7);
		spc.lowerlegsflag = 1 + floor(Stream.GetFraction() * 7);
		spc.upperlegsflag = 1 + floor(Stream.GetFraction() * 7);
		spc.red = floor(Stream.GetFraction() * 256);
		spc.green = floor(Stream.GetFraction() * 256);
		spc.blue = floor(Stream.GetFraction() * 256);
		spc.animalscale = 0.5 + Stream.GetFraction();
		spc.bodyscale = 1.0 + Stream.GetFraction();
		spc.headscale = 1.0 + Stream.GetFraction();
		spc.tailscale = 1.0 + Stream.GetFraction();
		spc.legsscale = 1.0 + Stream.GetFraction();
		spc.food = 10.0 + Stream.GetFraction() * 10.0;
		spc.material = 10.0 + Stream.GetFraction() * 10.0;
		spc.life = 1.0 + Stream.GetFraction() * 3.0;
		spc.agresive = Stream.GetFraction() > 0.5 ? true : false;
		spc.name = FirstName[floor(Stream.GetFraction() * FirstName.Num())] + " "
			+ SecondName[floor(Stream.GetFraction() * SecondName.Num())] + " "
			+ ThirdName[floor(Stream.GetFraction() * ThirdName.Num())];
		spc.speciesNumber = i;
		Species.Add(spc);
	}
}

