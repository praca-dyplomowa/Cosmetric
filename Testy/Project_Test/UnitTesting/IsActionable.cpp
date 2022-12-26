#include "pch.h"
#include "IsActionable.h"

// Sets default values
AIsActionable::AIsActionable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->bCanEverTick = true;
	Action = false;

}

// Called every frame
void AIsActionable::Tick(float DeltaTime)
{
}