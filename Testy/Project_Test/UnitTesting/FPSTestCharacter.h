#pragma once
class AFPSTestCharacter
{

public:
	AFPSTestCharacter();

protected:
	virtual void BeginPlay();

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnUseItem OnUseItem;
protected:

	/** Fires a projectile. */
	void OnPrimaryAction();
	//Movement Package
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		bool CameraLock;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void AddControllerYawInput(float Val) override;
	void AddControllerPitchInput(float Val) override;
	void Jump() override;
	void StopJumping() override;

	//touch pack
	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	//Tick Event
	void Tick(float DeltaTime);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int> AnimalCatalog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, UClass*> TreeCatalogClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FVector> TreeCatalogLocation;
	UFUNCTION()
		void ShowCatalog();
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<class AActor> CatalogViewClass;
	UPROPERTY(BlueprintReadWrite)
		AActor* CatalogView;

	//Equipment
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Food;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Wood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimalMaterial;

protected:
	TSubclassOf<class UPlayer_HUD_Widget> HUDClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPlayer_HUD_Widget* HUD;

	TSubclassOf<class UInGameMenu> PauseClass;
	class UInGameMenu* PauseMenu;
	void EnterMenu();

	TSubclassOf<class UBuildingMenu> BuildingClass;
	class UBuildingMenu* BuildingMenu;
	void EnterBuildMenu();
public:
	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;
	void ManageHealth(float dT);
	float Hunger;
	float HungerTimer;
	void EAT();
	void ManageHunger(float dT);
	float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TemperatureChange;
	UFUNCTION(BlueprintCallable)
		FCompactPlayerStats GetStats();
	UFUNCTION(BlueprintCallable)
		void SetStats(FCompactPlayerStats& stats);
	void ManageTemperature(float dT);

	//Tutorials

	TSubclassOf<class UTutorial> StartGameTutorialClass;
	class UTutorial* StartGameTutorial;
	TSubclassOf<class UTutorial> EatingTutorialClass;
	class UTutorial* EatingGameTutorial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorials")
		TSubclassOf<class UTutorial> NightTutorialClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorials")
		class UTutorial* NightGameTutorial;
	TSubclassOf<class UTutorial> HealthTutorialClass;
	class UTutorial* HealthGameTutorial;
	TSubclassOf<class UTutorial> RepetableTutorialClass;
	class UTutorial* RepetableGameTutorial;
	void ShowRepetableTutorial();


	//tutorial flags
	bool StartTutorialViewed;
	bool EatTutorialViewed;
	bool NightTutorialViewed;
	bool HelathTutorialViewed;
};

