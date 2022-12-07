// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IsCollectable.h"
#include "TestObject.generated.h"

UCLASS()
class FPSTEST_API ATestObject : public AIsCollectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* Cube;

};
