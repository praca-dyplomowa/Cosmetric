#pragma once
#include "IsActionable.h"

class AIsCollectable : public AIsActionable
{
public:
	// Sets default values for this actor's properties
	AIsCollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay() = 0;
	virtual void OnCollected();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float CollectTime;
	bool BeingCollected;
	bool Collectable;
	static const inline float MaxCollectTime = 3.0;
		int Materials[3];

	UAllPurposeProgressBarWidget* CollectProgressBar;

		void OnSelectedCollect(AActor* Target, FKey ButtonPressed);
		void OnUnselectedCollect(AActor* Target, FKey ButtonPressed);
		void OnEndCursorOverCollect(AActor* Target);

};

