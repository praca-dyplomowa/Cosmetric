#pragma once
#include <map>
#include <string>
#define UClass void
#include "FVector.h"

enum TSubclassOf {
	None,
	AActor,
	UPlayer_HUD_Widget,
	UInGameMenu,
	UBuildingMenu,
	UTutorial,
};

struct FCompactPlayerStats {


		std::string ToString();

		std::map<std::string, int> AnimalCatalog;

		std::map<std::string, UClass*> TreeCatalogClass;

		std::map<std::string, FVector> TreeCatalogLocation;

		float Food;

		float Wood;

		float AnimalMaterial;

		float Health;

		float Hunger;

		float Temperature;

		bool StartTutorialViewed;

		bool EatTutorialViewed;

		bool NightTutorialViewed;

		bool HelathTutorialViewed;
};

class AFPSTestCharacter
{

public:
	AFPSTestCharacter();

protected:
	virtual void BeginPlay();
	bool bCanEverTick;
public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float TurnRateGamepad;
protected:

	/** Fires a projectile. */
	void OnPrimaryAction();
	//Movement Package
	bool CameraLock;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void AddControllerYawInput(float Val);
	void AddControllerPitchInput(float Val);
	void Jump();
	void StopJumping();

	//Tick Event
	void Tick(float DeltaTime);

protected:
	// APawn interface
	//virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) ;
	//bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	//USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	//UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	bool Movement_Flag;
	//Catalog
	std::map<std::string, int> AnimalCatalog;
	std::map<std::string, UClass*> TreeCatalogClass;
	std::map<std::string, FVector> TreeCatalogLocation;
	void ShowCatalog();
	TSubclassOf CatalogViewClass;
	void* CatalogView;

	//Equipment
	float Food;
	float Wood;
	float AnimalMaterial;

protected:
	TSubclassOf HUDClass;
	class UPlayer_HUD_Widget* HUD;

	TSubclassOf PauseClass;
	class UInGameMenu* PauseMenu;
	void EnterMenu();

	TSubclassOf BuildingClass;
	class UBuildingMenu* BuildingMenu;
	void EnterBuildMenu();
public:
	//Stats
	float Health;
	void ManageHealth(float dT);
	float Hunger;
	float HungerTimer;
	void EAT();
	void ManageHunger(float dT);
	float Temperature;

	float TemperatureChange;
	FCompactPlayerStats GetStats();
	void SetStats(FCompactPlayerStats& stats);
	void ManageTemperature(float dT);

	//Tutorials

	TSubclassOf StartGameTutorialClass;
	class UTutorial* StartGameTutorial;
	TSubclassOf EatingTutorialClass;
	class UTutorial* EatingGameTutorial;
	TSubclassOf NightTutorialClass;
	class UTutorial* NightGameTutorial;
	TSubclassOf HealthTutorialClass;
	class UTutorial* HealthGameTutorial;
	TSubclassOf RepetableTutorialClass;
	class UTutorial* RepetableGameTutorial;
	void ShowRepetableTutorial();


	//tutorial flags
	bool StartTutorialViewed;
	bool EatTutorialViewed;
	bool NightTutorialViewed;
	bool HelathTutorialViewed;
};

