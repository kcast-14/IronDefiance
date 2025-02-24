// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IDEnums.h"
#include "IDPlayerController.generated.h"

/**
 * TODO: Organize this class a bit better
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnModeSwitch, ECameraMode, Mode, APawn*, Owner, ACharacterBase*, Tank);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotate, float, RotateVal);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Settings Menu Widget Class"))
	TSubclassOf<UUserWidget> m_WSettingsMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Settings Menu Widget Instance"))
	UUserWidget* m_SettingsMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Controls Menu Widget Class"))
	TSubclassOf<UUserWidget> m_WControlMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Controls Menu Widget Instance"))
	UUserWidget* m_ControlMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WMainMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Ultimate Screen Widget Class"))
	TSubclassOf<UUserWidget> m_WUltScreen;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Ultimate Screen Widget Instance"))
	UUserWidget* m_UltScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Command Menu Widget Class"))
	TSubclassOf<UUserWidget> m_WCommandMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Command Menu Widget Instance"))
	UUserWidget* m_CommandMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Win Screen Widget Class"))
	TSubclassOf<UUserWidget> m_WWinScreen;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Win Screen Widget Instance"))
	UUserWidget* m_WinScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Lose Screen Widget Class"))
	TSubclassOf<UUserWidget> m_WLoseScreen;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (DisplayName = "Lose Screen Widget Instance"))
	UUserWidget* m_LoseScreen;


	bool bHUDOverlayVisible = false;

	bool bEnemyHealthBarVisible = false;
	
	bool bTankHealthBarVisible = false;

	bool bPauseMenuVisible = false;

	bool bSettingsMenuVisible = false;
	
	bool bControlsMenuVisible = false;

	bool bMainMenuVisible = false;

	bool bWaveTransitionVisible = false;

	bool bOperatorHUDVisible = false;
	
	bool bSniperHUDVisible = false;

	bool bActionHUDVisible = false;

	bool bUltScreenVisible = false;

	bool bIsTopDown = false;

	bool bCommandMenuVisible = false;
	
	bool bLoseScreenVisible = false;

	bool bWinScreenVisible = false;

	UPROPERTY()
	FVector TankLocation;

	UPROPERTY()
	TMap<AEnemy*, FVector> m_EnemyLocations;

	UPROPERTY()
	int m_PoolSize = 0;

	FOnModeSwitch m_OnModeSwitch;

	FOnRotate m_OnRotate;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	virtual bool Raycast(FVector StartLocation, FVector Direction, FHitResult& HitResult);

public:
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();
	void DisplayTankHealthBar();
	void RemoveTankHealthBar();

	void DisplayWaveTransition();
	void RemoveWaveTransition();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayUltScreen();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveUltScreen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayCommandMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveCommandMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayWinScreen();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveWinScreen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayLoseScreen();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveLoseScreen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayMainMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveMainMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayControlsMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveControlsMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplaySettingsMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveSettingsMenu();

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
	void ToggleUltimateScreen();
	UFUNCTION()
	void ToggleCommandMenu();
	UFUNCTION()
	void ToggleWinScreen();
	UFUNCTION()
	void ToggleLoseScreen();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void TogglePauseMenu();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleControlsMenu();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleSettingsMenu();
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

	AOperatorPawn* GetOperatorPointer() { return m_Operator; }

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
	void OpenCommandMenu(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Turn(const FInputActionValue& Value);

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
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Command Menu Action"))
		UInputAction* m_CommandMenuAction = nullptr;
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

	void EnterActionMode();

};
