// Fill out your copyright notice in the Description page of Project Settings.


#include "IsScannable.h"
#include "../FPSTestCharacter.h"

// Sets default values
AIsScannable::AIsScannable()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	// Load the Cube mesh
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	UMaterial* mt = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Ground_Grass"));
	// Set the component's mesh
	cubeMeshComponent->SetStaticMesh(cubeMesh);
	cubeMeshComponent->SetMaterial(0, mt);
	OnClicked.AddUniqueDynamic(this, &AIsScannable::OnSelected);
	OnReleased.AddUniqueDynamic(this, &AIsScannable::OnUnselected);
}

// Called when the game starts or when spawned
void AIsScannable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsScannable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AIsScannable::OnSelected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Bump"));
	((AFPSTestCharacter *)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = false;
}

void AIsScannable::OnUnselected(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Bump2"));
	((AFPSTestCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()))->Movement_Flag = true;
}