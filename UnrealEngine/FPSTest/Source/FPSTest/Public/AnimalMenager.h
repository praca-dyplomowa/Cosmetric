// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimalBase.h"
#include <random>
#include "AnimalMenager.generated.h"

USTRUCT(BlueprintType)
struct FSpecies
{
	GENERATED_BODY()
public:

	FSpecies()
	{
		bodyflag = 1;
		headflag = 1;
		legsflag = 1;
		tailflag = 1;

		animalscale = 1.0;
		bodyscale = 1.0;
		headscale = 1.0;
		tailscale = 1.0;
		legsscale = 1.0;

		red = 0;
		blue = 0;
		green = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int bodyflag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int headflag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int legsflag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int tailflag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float animalscale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bodyscale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float headscale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float tailscale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float legsscale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float life;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString name;
};

UCLASS()
class FPSTEST_API AAnimalMenager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimalMenager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	static inline int _headtypes = 2;
	static inline int _bodytypes = 2;
	static inline int _tailtypes = 2;
	static inline int _legstypes = 2;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSpecies> Species;

	UFUNCTION(BlueprintCallable)
		void init(int Seed);

	UPROPERTY()
		TSubclassOf<class AAnimalBase> AnimalClass;

	UPROPERTY()
		float AnimalSpawnTimer;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

};
