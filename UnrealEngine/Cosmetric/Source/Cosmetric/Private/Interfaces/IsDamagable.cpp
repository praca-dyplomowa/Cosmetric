// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IsDamagable.h"
#include "UObject/UObjectGlobals.h"

AIsDamagable::AIsDamagable()
{
	OnClicked.AddUniqueDynamic(this, &AIsDamagable::OnSelectedDamage);
	Alive = false;
	BeingDamaged = false;
}

void AIsDamagable::BeginPlay()
{
	Super::BeginPlay();
}

void AIsDamagable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AIsDamagable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BeingDamaged)
	{
		if (DamageTime <= 0)
		{
			BeingDamaged = false;
			Action = false;
		}
		DamageTime -= DeltaTime;
	}
}

void AIsDamagable::OnSelectedDamage(AActor* Target, FKey ButtonPressed)
{
	if (EKeys::LeftMouseButton == ButtonPressed)
	{
		if (700 >= this->GetDistanceTo(((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))))
		{
			if (Alive && !BeingDamaged)
			{
				OnDamageTaken();
				Health -= 1.0;
				if (Health > 0.0)
				{
					BeingDamaged = true;
					DamageTime = 1.0;
					Action = true;
				}
				else
				{
					OnKilled();
				}
			}
		}
	}
}

void AIsDamagable::OnKilled()
{
	Alive = false;
	Action = true;
	BeingDamaged = true;
	DamageTime = 0.1;
}

void AIsDamagable::OnDamageTaken()
{
}
