// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IDEnums.h"
#include "IDPlayerController.generated.h"

/**
 * TODO: Organize this class a bit better
 */

class AEnemy;
class ACharacterBase;
class AOperatorPawn;
class UUserWidget;
class UInputAction;
class UInputMappingContext;
class USoundCue;
struct FInputActionValue;

UCLASS()
class IRONDEFIANCE_API AIDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Health Bar Widget Class"))
	TSubclassOf<UUserWidget> m_WHealthBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Enemy Health Bar Widget Instance"))
	TArray<UUserWidget*> m_EnemyHealthBars;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Tank Health Bar Widget Instance"))
	UUserWidget* m_TankHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Wave Transition Widget Class"))
	TSubclassOf<UUserWidget> m_WWaveTransition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Wave Transition Widget Instance"))
	UUserWidget* m_WaveTransition;
	/**Reference to the UMG Asset in the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Operator Mode Widget Class"))
	TSubclassOf<UUserWidget> m_WOperatorHUD;
	/**Variable to hold the Widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Operator Mode Widget Instance"))
	UUserWidget* m_OperatorHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Sniper HUD Widget Class"))
	TSubclassOf<UUserWidget> m_WSniperHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Sniper HUD Widget Instance"))
	UUserWidget* m_SniperHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Action HUD Widget Class"))
	TSubclassOf<UUserWidget> m_WActionHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Action HUD Widget Instance"))
	UUserWidget* m_ActionHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WMainMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* MainMenu;


	bool bHUDOverlayVisible = false;

	bool bEnemyHealthBarVisible = false;
	
	bool bTankHealthBarVisible = false;

	bool bPauseMenuVisible = false;

	bool bMainMenuVisible = false;

	bool bWaveTransitionVisible = false;

	bool bOperatorHUDVisible = false;
	
	bool bSniperHUDVisible = false;

	bool bActionHUDVisible = false;

	bool bIsTopDown = false;

	FVector TankLocation;

	TMap<AEnemy*, FVector> m_EnemyLocations;

	int m_PoolSize = 0;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	virtual bool Raycast();
	virtual AActor* Raycast(FVector StartLocation, FVector Direction);

public:
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();
	void DisplayTankHealthBar();
	void RemoveTankHealthBar();

	void DisplayWaveTransition();
	void RemoveWaveTransition();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayMainMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveMainMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayOperatorHUD();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveOperatorHUD();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplaySniperHUD();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveSniperHUD();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayActionHUD();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveActionHUD();

	UFUNCTION()
	void TogglePauseMenu();
	UFUNCTION()
	void ToggleOperatorHUD();
	UFUNCTION()
	void ToggleSniperHUD();
	UFUNCTION()
	void ToggleActionHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleMainMenu();

	//TODO: Determine if I really need these
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EnterSniperMode();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ExitSniperMode();

	//There is another Blueprint Callable function like this in ACharacterBase however,
	//we will only keep one of these, and that will be determined by which workflow is easiest.
	//For now both of them are here and we shall experiement with them in the UI later
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Upgrade")
	void UpgradeTank(ACharacterBase* TankToUpgrade, float Value, EUpgradeType Type = EUpgradeType::DEFAULT_MAX);



	UFUNCTION()
	void MakeHealthBarWidgets();
	UFUNCTION()
	void SetPoolSize(int Size);
	UFUNCTION()
	void AddEnemyLocation(AEnemy* Enemy, const FVector Loc);
	UFUNCTION()
	void RemoveEnemyLocation(AEnemy* Enemy);

	void SetOperatorPointer(AOperatorPawn* Ptr) { m_Operator = Ptr; }

	bool IsHittingAnotherTank(AActor* Actor);

	bool IsHittingEnemy(AActor* Actor);

	bool IsHittingTankOrEnemy(AActor* Actor);


public:

	//Input Mapping Callbacks
	UFUNCTION()
	void Jump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJumping(const FInputActionValue& Value);
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	void PauseGame(const FInputActionValue& Value);
	UFUNCTION()
	void SwitchToSniper(const FInputActionValue& Value);
	UFUNCTION()
	void SwitchToOperator(const FInputActionValue& Value);
	UFUNCTION()
	void SwitchToAction(const FInputActionValue& Value);
	UFUNCTION()
	void SwitchTanks(const FInputActionValue& Value);
	UFUNCTION()
	void Zoom(const FInputActionValue& Value);
	UFUNCTION()
	void Select(const FInputActionValue& Value);



private:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Player Mapping Context"))
		UInputMappingContext* m_PlayerMappingContext;

		//Action Mapping Variables
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Jump Action"))
		UInputAction* m_JumpAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Move Action"))
		UInputAction* m_MoveAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Look Action"))
		UInputAction* m_LookAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Turn Action"))
		UInputAction* m_TurnAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Pause Action"))
		UInputAction* m_PauseAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Switch To Tactician Mode"))
		UInputAction* m_SwitchToOperator = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Switch To Action Mode"))
		UInputAction* m_SwitchToAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Switch To Sniper Mode"))
		UInputAction* m_SwitchToSniper = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Switch Tanks"))
		UInputAction* m_SwitchTank = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Zoom Action"))
		UInputAction* m_ZoomAction = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Select Action"))
		UInputAction* m_SelectAction = nullptr;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Operator", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Operator Pointer"))
		AOperatorPawn* m_Operator;

		UPROPERTY()
		TArray<ACharacterBase*> m_Tanks;

		UPROPERTY()
		int m_CurrentControlledTank = 0;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Pause Sound"))
		USoundCue* m_Pause;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Back Sound"))
		USoundCue* m_Back;


private:

	void PlaceTank(FVector Location, FVector Direction);

};
