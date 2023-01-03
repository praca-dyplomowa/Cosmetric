#pragma once
class AIsActionable
{

public:
	// Sets default values for this actor's properties
	AIsActionable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime);

	bool Action;

	bool bCanEverTick;
};

