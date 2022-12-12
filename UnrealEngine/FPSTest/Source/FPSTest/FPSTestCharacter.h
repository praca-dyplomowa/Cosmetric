// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/HUD/Player_HUD_Widget.h"
#include "Blueprint/UserWidget.h"
#include "Public/GUI/InGameMenu.h"
#include "Public/GUI/BuildingMenu.h"
#include "FPSTestCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

USTRUCT(BlueprintType)
struct FCompactPlayerStats {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		TArray<FString> Catalog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Wood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimalMaterial;

	float Health;
	float Hunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Temperature;
};

UCLASS(config=Game)
class AFPSTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFPSTestCharacter();

protected:
	virtual void BeginPlay();

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;
protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();
//Movement Package
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void AddControllerYawInput(float Val) override;
	void AddControllerPitchInput(float Val) override;
	void Jump() override;
	void StopJumping() override;
	void Tick(float DeltaTime);


	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	bool Movement_Flag;
	//Catalog
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Catalog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Wood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimalMaterial;

	TSubclassOf<class UPlayer_HUD_Widget> HUDClass;
	TSubclassOf<class UInGameMenu> PauseClass;
	TSubclassOf<class UBuildingMenu> BuildingClass;

	UPROPERTY()
		class UPlayer_HUD_Widget* HUD;

	UPROPERTY()
		class UInGameMenu* PauseMenu;
	UPROPERTY()
		class UBuildingMenu* BuildingMenu;

	//Stats
	float Health;
	float Hunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TemperatureChange;

	void EAT();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool menuing;

	void EnterMenu();
	void EnterBuildMenu();
	FCompactPlayerStats GetStats();
	void SetStats(FCompactPlayerStats& stats);
};

