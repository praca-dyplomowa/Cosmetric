// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestCharacter.h"
#include "FPSTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "../Public/Singleton.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Player_HUD_Widget.h"
#include "GameFramework/InputSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AFPSTestCharacter

AFPSTestCharacter::AFPSTestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	HUD = nullptr;
	HUDClass = nullptr;
	//PauseClass = nullptr;
	//PauseMenu = nullptr;
	Health = 100.0;
	Hunger = 100.0;
	Equipment[0] = 100.0;
	Temperature = 0.0;
}

void AFPSTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	APlayerController* ctr = GetWorld()->GetFirstPlayerController();
	check(ctr);
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleton::StaticClass(), ActorsToFind);
	for (AActor* Singleton : ActorsToFind)
	{
		ASingleton* single = Cast<ASingleton>(Singleton);
		if (single)
		{
			HUDClass = single->HUDClass;
			PauseClass = single->PauseClass;
		}
	}
	if (HUDClass)
	{
		HUD = CreateWidget<UPlayer_HUD_Widget>(ctr, HUDClass);
		check(HUD);
		HUD->AddToPlayerScreen();
		HUD->SetTemp(0.0);
		HUD->SetHealth(100.0);
		HUD->SetHunger(100.0);
		HUD->SetHunger(100.0);
		HUD->SetVisibility(ESlateVisibility::Visible);
	}
	if (PauseClass)
	{
		PauseMenu = CreateWidget<UInGameMenu>(ctr, PauseClass);
		check(PauseMenu);
		PauseMenu->AddToPlayerScreen();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Movement_Flag = true;
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSTestCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSTestCharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSTestCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &AFPSTestCharacter::OnPrimaryAction);

	PlayerInputComponent->BindAction("EAT", IE_Pressed, this, &AFPSTestCharacter::EAT);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AFPSTestCharacter::EnterMenu);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AFPSTestCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AFPSTestCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFPSTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFPSTestCharacter::LookUpAtRate);

	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
	GetWorld()->GetFirstPlayerController()->ClickEventKeys.Add(EKeys::RightMouseButton);
	GetWorld()->GetFirstPlayerController()->ClickEventKeys.Add(EKeys::LeftMouseButton);
	menuing = true;
}

void AFPSTestCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AFPSTestCharacter::EAT()
{
	if (90.0 >= Hunger)
	{
		if (10.0 <= Equipment[0])
		{
			Equipment[0] -= 10.0;
			Hunger += 10.0;
		}
		else
		{
			if (0.0 <= Equipment[0])
			{
				Hunger += Equipment[0];
				Equipment[0] = 0.0;
			}
		}
	}
	else
	{
		if (100.0 - Hunger <= Equipment[0])
		{
			Equipment[0] -= 100 - Hunger;
			Hunger = 100.0;
		}
		else
		{
			Hunger += Equipment[0];
			Equipment[0] = 0.0;
		}
	}
}

void AFPSTestCharacter::EnterMenu()
{
	if (!menuing)
	{
		this->menuing = true;
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		if (PauseMenu)
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		this->menuing = false;
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		if (PauseMenu)
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}

void AFPSTestCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSTestCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AFPSTestCharacter::MoveForward(float Value)
{
	if (Movement_Flag && Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSTestCharacter::MoveRight(float Value)
{
	if (Movement_Flag && Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSTestCharacter::AddControllerYawInput(float Val)
{
	if (Movement_Flag)
		Super::AddControllerYawInput(Val);
}

void AFPSTestCharacter::AddControllerPitchInput(float Val)
{
	if (Movement_Flag)
		Super::AddControllerPitchInput(Val);
}

void AFPSTestCharacter::Jump()
{
	if (Movement_Flag)
		Super::Jump();
}

void AFPSTestCharacter::StopJumping()
{
	if (Movement_Flag)
		Super::StopJumping();
}

void AFPSTestCharacter::TurnAtRate(float Rate)
{
	if(Movement_Flag)
		Super::AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSTestCharacter::LookUpAtRate(float Rate)
{
	if (Movement_Flag)
		Super::AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int32 x, y;
	if (!menuing)
	{
		GetWorld()->GetFirstPlayerController()->GetViewportSize(x, y);
		GetWorld()->GetFirstPlayerController()->SetMouseLocation(x / 2, y / 2);
		Hunger -= DeltaTime;
		if (0.0 >= Hunger)
		{
			Health -= DeltaTime;
		}
		if (HUD)
		{
			HUD->SetHealth(Health);
			HUD->SetHunger(Hunger);
			HUD->SetFood(Equipment[0]);
			HUD->SetTemp(Temperature);
		}
	}
}

bool AFPSTestCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSTestCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSTestCharacter::EndTouch);

		return true;
	}
	
	return false;
}