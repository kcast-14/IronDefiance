// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/IDPlayerController.h"
#include "Actors/Wave.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CharacterBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"
#include "GameInstance/IDGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/OperatorPawn.h"
#include "Sound/SoundCue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

void AIDPlayerController::DisplayEnemyHealthBar()
{
	for (auto& W : m_EnemyHealthBars)
	{
		bEnemyHealthBarVisible = true;
		W->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIDPlayerController::RemoveEnemyHealthBar()
{
	for (auto& W : m_EnemyHealthBars)
	{
		bEnemyHealthBarVisible = false;
		W->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AIDPlayerController::DisplayTankHealthBar()
{
	if (m_TankHealthBar)
	{
		bTankHealthBarVisible = true;
		m_TankHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIDPlayerController::RemoveTankHealthBar()
{
	if (m_TankHealthBar)
	{
		bTankHealthBarVisible = false;
		m_TankHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AIDPlayerController::DisplayWaveTransition()
{
	if (m_WaveTransition)
	{
		bWaveTransitionVisible = true;
		m_WaveTransition->SetVisibility(ESlateVisibility::Visible);
		IgnoreLookInput = true;
		IgnoreMoveInput = true;
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);;
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveWaveTransition()
{
	if (m_WaveTransition)
	{
		bWaveTransitionVisible = false;
		m_WaveTransition->SetVisibility(ESlateVisibility::Hidden);
		IgnoreLookInput = false;
		IgnoreMoveInput = false;

		bShowMouseCursor = true;
	}
}

void AIDPlayerController::DisplayCommandMenu_Implementation()
{
	if (m_CommandMenu)
	{
		bCommandMenuVisible = true;
		m_CommandMenu->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveCommandMenu_Implementation()
{
	if (m_CommandMenu)
	{
		bCommandMenuVisible = false;
		m_CommandMenu->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::DisplayUltScreen_Implementation()
{
	if (m_UltScreen)
	{
		bUltScreenVisible = true;
		m_UltScreen->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveUltScreen_Implementation()
{
	//Whether or not we show the mouse cursor will have to be determined by which HUD they had displayed at the time of using the Ult

	if (m_UltScreen)
	{
		bUltScreenVisible = false;
		m_UltScreen->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplayWinScreen_Implementation()
{
	if (m_WinScreen)
	{
		bWinScreenVisible = true;
		m_WinScreen->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveWinScreen_Implementation()
{
	if (m_WinScreen)
	{
		bWinScreenVisible = false;
		m_WinScreen->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplayLoseScreen_Implementation()
{
	if (m_LoseScreen)
	{
		bLoseScreenVisible = true;
		m_LoseScreen->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveLoseScreen_Implementation()
{
	if (m_LoseScreen)
	{
		bLoseScreenVisible = false;
		m_LoseScreen->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplayLoadingScreen_Implementation()
{
	if (m_LoadingScreen)
	{
		bLoadingScreenVisible = true;
		m_LoadingScreen->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::RemoveLoadingScreen_Implementation()
{
	if (m_LoadingScreen)
	{
		bLoadingScreenVisible = false;
		m_LoadingScreen->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}


void AIDPlayerController::DisplayMainMenu_Implementation()
{
	if (MainMenu)
	{
		bMainMenuVisible = true;
		MainMenu->SetVisibility(ESlateVisibility::Visible);
		IgnoreLookInput = true;
		IgnoreMoveInput = true;
		//MainMenu->GetWidgetFromName("NewGameButton")->SetUserFocus(this);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}

}

void AIDPlayerController::RemoveMainMenu_Implementation()
{
	if (MainMenu)
	{
		bMainMenuVisible = false;
		MainMenu->SetVisibility(ESlateVisibility::Hidden);
		IgnoreLookInput = false;
		IgnoreMoveInput = false;
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
	//	PauseMenu->GetWidgetFromName("ResumeButton")->SetUserFocus(this);
		IgnoreLookInput = true;
		IgnoreMoveInput = true;
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = false;
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		//if (!bLoadGameScreenVisible && !bSaveGameMenuVisible)
		//{
		//	IgnoreLookInput = false;
		//	IgnoreMoveInput = false;
		//	SetInputMode(InputModeGameOnly);
		//	bShowMouseCursor = false;
		//}

	}

}

void AIDPlayerController::DisplayControlsMenu_Implementation()
{
	if (m_ControlMenu)
	{
		bControlsMenuVisible = true;
		m_ControlMenu->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveControlsMenu_Implementation()
{
	if (m_ControlMenu)
	{
		bControlsMenuVisible = false;
		m_ControlMenu->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::DisplaySettingsMenu_Implementation()
{
	if (m_SettingsMenu)
	{
		bSettingsMenuVisible = true;
		m_SettingsMenu->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveSettingsMenu_Implementation()
{
	if (m_OperatorHUD)
	{
		bSettingsMenuVisible = false;
		m_SettingsMenu->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::DisplayOperatorHUD_Implementation()
{
	if (m_OperatorHUD)
	{
		bOperatorHUDVisible = true;
		m_OperatorHUD->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameUI;
		SetInputMode(InputModeGameUI);
		bShowMouseCursor = true;
	}
}

void AIDPlayerController::RemoveOperatorHUD_Implementation()
{
	if (m_OperatorHUD)
	{
		bOperatorHUDVisible = false;
		m_OperatorHUD->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplaySniperHUD_Implementation()
{
	if (m_SniperHUD)
	{
		bSniperHUDVisible = true;
		m_SniperHUD->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::RemoveSniperHUD_Implementation()
{
	if (m_SniperHUD)
	{
		bSniperHUDVisible = false;
		m_SniperHUD->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::DisplayActionHUD_Implementation()
{
	if (m_ActionHUD)
	{
		bActionHUDVisible = true;
		m_ActionHUD->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::RemoveActionHUD_Implementation()
{
	if (m_ActionHUD)
	{
		bActionHUDVisible = false;
		m_ActionHUD->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AIDPlayerController::ToggleUltimateScreen()
{
	if (bUltScreenVisible)
	{
		RemoveUltScreen();
	}
	else
	{
		DisplayUltScreen();
	}
}

void AIDPlayerController::ToggleCommandMenu()
{

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		if (bCommandMenuVisible)
		{
			RemoveCommandMenu();
		}
		else
		{
			DisplayCommandMenu();
		}
	}
	case ECameraMode::CM_ActionMode:
	case ECameraMode::CM_SniperMode:
	default:
	{
		break;
	}
	}

}

void AIDPlayerController::ToggleWinScreen()
{
	if (bWinScreenVisible)
	{
		RemoveWinScreen();
	}
	else
	{
		DisplayWinScreen();
	}
}

void AIDPlayerController::ToggleLoseScreen()
{
	if (bLoseScreenVisible)
	{
		RemoveLoseScreen();
	}
	else
	{
		DisplayLoseScreen();
	}
}

void AIDPlayerController::ToggleLoadingScreen()
{
	if (bLoadingScreenVisible)
	{
		RemoveLoadingScreen();
	}
	else
	{
		DisplayLoadingScreen();
	}
}

void AIDPlayerController::TogglePauseMenu()
{
	//For right now it seems that we can't display 2 HUDs AND interact with the one that should be "on top", therefore we have to do this
	if (bPauseMenuVisible)
	{
		UGameplayStatics::PlaySound2D(this, m_Back);
		RemovePauseMenu();
		switch (m_Operator->GetCameraMode())
		{
		case ECameraMode::CM_TacticianMode:
		{
			ToggleOperatorHUD();
			break;
		}
		case ECameraMode::CM_ActionMode:
		{
			ToggleActionHUD();
			break;
		}
		case ECameraMode::CM_SniperMode:
		{
			ToggleSniperHUD();
			break;
		}
		default:
			break;
		}

	}
	else
	{
		UGameplayStatics::PlaySound2D(this, m_Pause);
		switch (m_Operator->GetCameraMode())
		{
		case ECameraMode::CM_TacticianMode:
		{
			ToggleOperatorHUD();
			break;
		}
		case ECameraMode::CM_ActionMode:
		{
			ToggleActionHUD();
			break;
		}
		case ECameraMode::CM_SniperMode:
		{
			ToggleSniperHUD();
			break;
		}
		default:
			break;
		}
		DisplayPauseMenu();
	}
}

void AIDPlayerController::ToggleControlsMenu()
{
	if (bControlsMenuVisible)
	{

	}
	else
	{

	}
}

void AIDPlayerController::ToggleSettingsMenu()
{
	if (bSettingsMenuVisible)
	{

	}
	else
	{

	}
}

void AIDPlayerController::ToggleOperatorHUD()
{
	if (bOperatorHUDVisible)
	{
		RemoveOperatorHUD();
	}
	else
	{
		DisplayOperatorHUD();
	}
}

void AIDPlayerController::ToggleSniperHUD()
{
	if (bSniperHUDVisible)
	{

		RemoveSniperHUD();
	}
	else
	{
		DisplaySniperHUD();
	}
}

void AIDPlayerController::ToggleActionHUD()
{
	if (bActionHUDVisible)
	{
		RemoveActionHUD();
	}
	else
	{
		DisplayActionHUD();
	}
}

void AIDPlayerController::ToggleMainMenu()
{
	if (bMainMenuVisible)
	{
		RemoveMainMenu();
	}
	else
	{
		DisplayMainMenu();
	}
}

void AIDPlayerController::EnterSniperMode()
{
	ToggleSniperHUD();
}

void AIDPlayerController::ExitSniperMode()
{
	ToggleSniperHUD();
}

void AIDPlayerController::UpgradeTank(ACharacterBase* TankToUpgrade, float Value, EUpgradeType Type)
{
	TankToUpgrade->ApplyUpgrade(Value, Type);
}

void AIDPlayerController::SwitchTanks(const FInputActionValue& Value)
{
	if (m_Tanks.IsEmpty())
	{
		return;
	}

	//Basically we'll cycle through the array going to the right until we reach the end, once we reach the end
	//We'll reset back to the first tank and the player will have to keep cycling.
	if (m_CurrentControlledTank <= m_Tanks.Num() -1 )
	{
		m_CurrentControlledTank++;
		m_Operator->SetTankToPilot(m_Tanks[m_CurrentControlledTank]);
		Possess(m_Operator->GetTankToPilot());
		return;
	}

	m_CurrentControlledTank = 0;
	m_Operator->SetTankToPilot(m_Tanks[m_CurrentControlledTank]);
	Possess(m_Operator->GetTankToPilot());
}

void AIDPlayerController::MakeHealthBarWidgets()
{
	check(m_WHealthBar);
	FVector2D Alignment(0.f, 0.f);

	if (m_EnemyHealthBars.GetAllocatedSize() > 0)
	{
		m_EnemyHealthBars.Empty();
	}

	for (int i = 0; i < m_PoolSize; ++i)
	{
		m_EnemyHealthBars.Push(CreateWidget<UUserWidget>(this, m_WHealthBar));
	}

	for (auto& W : m_EnemyHealthBars) // For All Widgets (W) in EnemyHealthBars Array
	{
		check(W);
		W->AddToViewport();
		W->SetVisibility(ESlateVisibility::Hidden);
		W->SetAlignmentInViewport(Alignment);
	}
}

void AIDPlayerController::SetPoolSize(int Size)
{
	m_PoolSize = Size;
	m_EnemyLocations.Empty();
	m_EnemyLocations.Reserve(Size);
}

void AIDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FVector2D Alignment(0.f, 0.f);


	check(m_WWaveTransition);

	m_WaveTransition = CreateWidget<UUserWidget>(this, m_WWaveTransition);
	m_WaveTransition->AddToViewport();
	m_WaveTransition->SetVisibility(ESlateVisibility::Hidden);



	check(WPauseMenu);
	PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
	//Cast<UButton>(PauseMenu->GetWidgetFromName("LoadGameButton"))->OnClicked.AddDynamic(this, &AMainPlayerController::OnLoadGameClicked);
	//Cast<UButton>(PauseMenu->GetWidgetFromName("SaveGameButton"))->OnClicked.AddDynamic(this, &AMainPlayerController::OnSaveGameClicked);
	check(PauseMenu)
	PauseMenu->AddToViewport();
	PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	
	PauseMenu->SetAlignmentInViewport(Alignment);

	check(m_WOperatorHUD);
	m_OperatorHUD = CreateWidget<UUserWidget>(this, m_WOperatorHUD);

	check(m_OperatorHUD);

	m_OperatorHUD->AddToViewport();
	m_OperatorHUD->SetVisibility(ESlateVisibility::Hidden);
	check(m_WSniperHUD);

	m_SniperHUD = CreateWidget<UUserWidget>(this, m_WSniperHUD);

	check(m_SniperHUD);

	m_SniperHUD->AddToViewport();
	m_SniperHUD->SetVisibility(ESlateVisibility::Hidden);

	check(m_WActionHUD);

	m_ActionHUD = CreateWidget<UUserWidget>(this, m_WActionHUD);

	check(m_ActionHUD);

	m_ActionHUD->AddToViewport();
	m_ActionHUD->SetVisibility(ESlateVisibility::Hidden);

	check(m_WUltScreen);
	m_UltScreen = CreateWidget<UUserWidget>(this, m_WUltScreen);

	check(m_UltScreen);

	m_UltScreen->AddToViewport();
	m_UltScreen->SetVisibility(ESlateVisibility::Hidden);

	check(m_WCommandMenu);

	m_CommandMenu = CreateWidget<UUserWidget>(this, m_WCommandMenu);

	check(m_CommandMenu);

	m_CommandMenu->AddToViewport();
	m_CommandMenu->SetDesiredSizeInViewport({ 512.f, 512.f });
	m_CommandMenu->SetVisibility(ESlateVisibility::Hidden);


	check(m_WWinScreen);

	m_WinScreen = CreateWidget<UUserWidget>(this, m_WWinScreen);

	check(m_WinScreen);
	m_WinScreen->AddToViewport();
	m_WinScreen->SetVisibility(ESlateVisibility::Hidden);

	check(m_WLoseScreen);
	m_LoseScreen = CreateWidget<UUserWidget>(this, m_WLoseScreen);
	
	check(m_LoseScreen);
	m_LoseScreen->AddToViewport();
	m_LoseScreen->SetVisibility(ESlateVisibility::Hidden);

	check(m_WLoadingScreen);
	m_LoadingScreen = CreateWidget<UUserWidget>(this, m_WLoadingScreen);

	check(m_LoadingScreen);
	m_LoadingScreen->AddToViewport();
	m_LoadingScreen->SetVisibility(ESlateVisibility::Hidden);

	check(m_WSettingsMenu);
	m_SettingsMenu = CreateWidget<UUserWidget>(this, m_WSettingsMenu);

	check(m_SettingsMenu);
	m_SettingsMenu->AddToViewport();
	m_SettingsMenu->SetVisibility(ESlateVisibility::Hidden);

	check(m_WControlMenu);
	m_ControlMenu = CreateWidget<UUserWidget>(this, m_WControlMenu);

	check(m_ControlMenu);
	m_ControlMenu->AddToViewport();
	m_ControlMenu->SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(m_PlayerMappingContext, 0);
	}

	if (GetGameInstance<UIDGameInstance>()->GetWavePtr() != nullptr)
	{
		DisplayWaveTransition();
		GetGameInstance<UIDGameInstance>()->GetWavePtr()->EnterTransition();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}

	
}


void AIDPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_EnemyHealthBars.Num() > 0)
	{
		int Index = 0;

		for (auto& L : m_EnemyLocations)
		{

			FVector2D PositionInViewport;
			ProjectWorldLocationToScreen(L.Get<FVector>(), PositionInViewport);
			PositionInViewport.Y -= 50.f;
			PositionInViewport.X -= 35.f;

			FVector2D SizeInViewport = FVector2D(200.f, 25.f);

			m_EnemyHealthBars[Index]->SetPositionInViewport(PositionInViewport);
			m_EnemyHealthBars[Index]->SetDesiredSizeInViewport(SizeInViewport);
			Index++;
		}
	}
}

void AIDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		//I know we don't have jumping, but this is left here until I know for sure we don't need it.
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Started, this, &AIDPlayerController::Jump);
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &AIDPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Move);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Look);
		EnhancedInputComponent->BindAction(m_TurnAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Turn);
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Triggered, this, &AIDPlayerController::PauseGame);
		EnhancedInputComponent->BindAction(m_CommandMenuAction, ETriggerEvent::Triggered, this, &AIDPlayerController::OpenCommandMenu);
		EnhancedInputComponent->BindAction(m_SwitchTank, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchTanks);
		EnhancedInputComponent->BindAction(m_SwitchToOperator, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchToOperator);
		EnhancedInputComponent->BindAction(m_SwitchToAction, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchToAction);
		EnhancedInputComponent->BindAction(m_SwitchToSniper, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchToSniper);
		EnhancedInputComponent->BindAction(m_ZoomAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Zoom);
		EnhancedInputComponent->BindAction(m_SelectAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Select); //Name is subject to change

		//m_PlayerMappingContext->GetMappings()[0].

	}
}

bool AIDPlayerController::Raycast(FVector StartLocation, FVector Direction, FHitResult& HitResult)
{
	float CastLength = 10000.f;

	FVector Location;
	FRotator Rotation;

	FVector EndLocation = StartLocation + (Direction * CastLength);
	
	FCollisionQueryParams CollisionParams;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		return true;
	}

	return false;
}

void AIDPlayerController::AddEnemyLocation(AEnemy* Enemy, const FVector Loc)
{
	// This isn't going to work because there's no real way to say whether the enemy at this location will be alive
// In fact we're relying on it dying at some point.
	m_EnemyLocations.Emplace(Enemy, Loc);
}

void AIDPlayerController::RemoveEnemyLocation(AEnemy* Enemy)
{
	m_EnemyLocations.FindAndRemoveChecked(Enemy);
}

bool AIDPlayerController::IsHittingAnotherTank(AActor* Actor)
{
	if (Cast<ACharacterBase>(Actor))
	{
		return true;
	}

	return false;
}

bool AIDPlayerController::IsHittingEnemy(AActor* Actor)
{
	if (Cast<AEnemy>(Actor))
	{
		return true;
	}
	return false;
}

bool AIDPlayerController::IsHittingTankOrEnemy(AActor* Actor)
{
	if (Cast<AEnemy>(Actor))
	{
		return true;
	}

	if (Cast<ACharacterBase>(Actor))
	{
		return true;
	}

	return false;
}

void AIDPlayerController::Jump(const FInputActionValue& Value)
{
}

void AIDPlayerController::StopJumping(const FInputActionValue& Value)
{
}

void AIDPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		m_Operator->AddMovementInput(m_Operator->GetActorForwardVector(), MoveVector.Y);
		m_Operator->AddMovementInput(m_Operator->GetActorRightVector(), MoveVector.X);
		break;
	}
	case ECameraMode::CM_ActionMode:
	{
		if (IsInputKeyDown(EKeys::A) || IsInputKeyDown(EKeys::D))
		{
			break;
		}
		m_Operator->GetTankToPilot()->AddMovementInput(m_Operator->GetTankToPilot()->GetActorForwardVector(), (MoveVector.Y * m_Operator->GetTankToPilot()->GetCharacterMovement()->MaxWalkSpeed));
		m_Operator->GetTankToPilot()->AddMovementInput(m_Operator->GetTankToPilot()->GetActorRightVector(), (MoveVector.X * m_Operator->GetTankToPilot()->GetCharacterMovement()->MaxWalkSpeed));
		break;
	}
	case ECameraMode::CM_SniperMode:
		//We probably need to do something with a camera here but maybe not
	{
	}
	default:
	{
		break;
	}
	}


}

void AIDPlayerController::OpenCommandMenu(const FInputActionValue& Value)
{
	// For now it will just open and close the command menu
	// However, in the future we'll need to determine which tank was right clicked, if any, and open the command menu and all commands should be specific to that particular tank

	FVector2D PositionInViewport;
	GetMousePosition(PositionInViewport.X, PositionInViewport.Y);
	m_CommandMenu->SetPositionInViewport(PositionInViewport);
	ToggleCommandMenu();
}

void AIDPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	
	if (m_Operator->IsPilotingTank())
	{
		switch (m_Operator->GetCameraMode())
		{
		case ECameraMode::CM_ActionMode:
		{
			AddYawInput(LookVector.X);
			AddPitchInput(-LookVector.Y);
			return;
		}
		case ECameraMode::CM_SniperMode:
		{
			AddYawInput(LookVector.X);
			AddPitchInput(-LookVector.Y);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return;

}

void AIDPlayerController::Turn(const FInputActionValue& Value)
{
	float TurnValue = Value.Get<float>();

	m_OnRotate.Broadcast(TurnValue * 10.f);
}

void AIDPlayerController::PauseGame(const FInputActionValue& Value)
{
	TogglePauseMenu();
}

void AIDPlayerController::SwitchToSniper(const FInputActionValue& Value)
{
	if (m_Tanks.IsEmpty())
	{
		return;
	}

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		if (m_Operator->GetTankToPilot() != nullptr)
		{
			ToggleOperatorHUD();
			Possess(m_Operator->GetTankToPilot()->GetFPSPawn());
			m_Operator->SetCameraMode(ECameraMode::CM_SniperMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot()->GetFPSPawn(), m_Operator->GetTankToPilot());
			ToggleSniperHUD();
		}
		else
		{
			//If we're not currently operating a tank then we'll randomly select a tank from the array of tanks currently placed and possess one of those
			m_Operator->SetTankToPilot(m_Tanks[FMath::RandRange(0, (m_Tanks.Num()))]);
			ToggleOperatorHUD();
			Possess(m_Operator->GetTankToPilot()->GetFPSPawn());
			m_Operator->SetCameraMode(ECameraMode::CM_SniperMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot()->GetFPSPawn(), m_Operator->GetTankToPilot());
			ToggleSniperHUD();
		}
		break;
	}
	case ECameraMode::CM_ActionMode:
	{
		if (m_Operator->GetTankToPilot() != nullptr)
		{
			ToggleActionHUD();
			Possess(m_Operator->GetTankToPilot()->GetFPSPawn());
			m_Operator->SetCameraMode(ECameraMode::CM_SniperMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot()->GetFPSPawn(), m_Operator->GetTankToPilot());
			ToggleSniperHUD();
		}
		else
		{
			//If we're not currently operating a tank then we'll randomly select a tank from the array of tanks currently placed and possess one of those
			m_Operator->SetTankToPilot(m_Tanks[FMath::RandRange(0, (m_Tanks.Num()))]);
			ToggleActionHUD();
			Possess(m_Operator->GetTankToPilot()->GetFPSPawn());
			m_Operator->SetCameraMode(ECameraMode::CM_SniperMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot()->GetFPSPawn(), m_Operator->GetTankToPilot());
			ToggleSniperHUD();
		}
		break;
	}
	case ECameraMode::CM_SniperMode:
	{
		break;
	}
	default:
	{
		break;
	}
	}

}

void AIDPlayerController::SwitchToOperator(const FInputActionValue& Value)
{
	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		// We're already in Tactician mode so do nothing.
		break;
	}
	case ECameraMode::CM_ActionMode:
	{
		
		ToggleActionHUD();
		ToggleOperatorHUD();
		Possess(m_Operator);
		m_Operator->CanPilotTank(false);
		m_Operator->SetTankToPilot(nullptr);
		m_Operator->SetCameraMode(ECameraMode::CM_TacticianMode);
		m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator, m_Operator->GetTankToPilot());
		m_CurrentControlledTank = -1;
		break;
	}
	case ECameraMode::CM_SniperMode:
	{
		ToggleSniperHUD();
		ToggleOperatorHUD();
		Possess(m_Operator);
		m_Operator->CanPilotTank(false);
		m_Operator->SetTankToPilot(nullptr);
		m_Operator->SetCameraMode(ECameraMode::CM_TacticianMode);
		m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator, m_Operator->GetTankToPilot());
		m_CurrentControlledTank = -1;
		break;
	}
	default:
	{
		break;
	}
	}
}

void AIDPlayerController::SwitchToAction(const FInputActionValue& Value)
{
	//Supposedly crashing in shipping - Unable to reproduce
	if (m_Tanks.IsEmpty())
	{
		return;
	}

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		if (m_Operator->GetTankToPilot() != nullptr)
		{
			ToggleOperatorHUD();
			Possess(m_Operator->GetTankToPilot());
			m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot(), m_Operator->GetTankToPilot());
			ToggleActionHUD();
		}
		else
		{
			//If we're not currently operating a tank then we'll randomly select a tank from the array of tanks currently placed and possess one of those
			m_Operator->SetTankToPilot(m_Tanks[FMath::RandRange(0, (m_Tanks.Num()))]);
			ToggleOperatorHUD();
			Possess(m_Operator->GetTankToPilot());
			m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot(), m_Operator->GetTankToPilot());
			ToggleActionHUD();
		}
		break;
	}
	case ECameraMode::CM_ActionMode:
	{
		// We're already in Action mode so do nothing.
		break;
	}
	case ECameraMode::CM_SniperMode:
	{
		if (m_Operator->GetTankToPilot() != nullptr)
		{
			ToggleSniperHUD();
			Possess(m_Operator->GetTankToPilot());
			m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot(), m_Operator->GetTankToPilot());
			ToggleActionHUD();
		}
		else
		{
			//If we're not currently operating a tank then we'll randomly select a tank from the array of tanks currently placed and possess one of those
			m_Operator->SetTankToPilot(m_Tanks[FMath::RandRange(0, (m_Tanks.Num()))]);
			ToggleSniperHUD();
			Possess(m_Operator->GetTankToPilot());
			m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
			m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot(), m_Operator->GetTankToPilot());
			ToggleActionHUD();
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void AIDPlayerController::Zoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_ActionMode:
	{
		m_Operator->GetTankToPilot()->GetSpringArmComponent()->TargetArmLength += (ZoomValue * -50.f);
		break;
	}
	case ECameraMode::CM_SniperMode:
	{
		m_Operator->GetTankToPilot()->GetFPSPawn()->GetSpringArmComponent()->TargetArmLength += (ZoomValue * -50.f);
		break;

	}
	default:
	{
		break;
	}
	}
}

void AIDPlayerController::Select(const FInputActionValue& Value)
{
	FVector WorldLocation;
	FVector WorldDirection;

	if (m_Operator->IsPilotingTank())
	{
		//Here is where we'd do the shooting and stuff like that
		return;
	}



	// This will require some testing down the line for controller support, however I'm pretty certain that while in Game Mode the Mouse is always centered,
	// so on mouse and keyboard this should be fine.
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FHitResult Result;
		if (Raycast(WorldLocation, WorldDirection, Result))
		{
			bool DidHit = IsHittingTankOrEnemy(Result.GetActor());

			if (m_Operator->GetTankToPlace() != nullptr)
			{
				if (DidHit)
				{
					return;
				}
				else
				{
					PlaceTank(Result.ImpactPoint, WorldDirection);
				}
			}
			else
			{
				if (IsHittingAnotherTank(Result.GetActor()))
				{
					m_Operator->CanPilotTank(true);
					m_Operator->SetTankToPilot(Result.GetActor());
					m_CurrentControlledTank = m_Tanks.Find(Cast<ACharacterBase>(Result.GetActor())); // It should be noted that if you simply place tanks in the world this isn't going to work.
					EnterActionMode();
				}

			}
		}
		


	}
}

void AIDPlayerController::PlaceTank(FVector Location, FVector Direction)
{
	ACharacterBase* PlacedTank = Cast<ACharacterBase>(m_Operator->PlaceTank(Location, Direction));

	if (PlacedTank)
	{
		m_Tanks.Add(PlacedTank);
	}
}

void AIDPlayerController::EnterActionMode()
{

	switch (m_Operator->GetCameraMode())
	{
	case ECameraMode::CM_TacticianMode:
	{
		Possess(m_Operator->GetTankToPilot());
		ToggleOperatorHUD();
		ToggleActionHUD();
		m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
		m_OnModeSwitch.Broadcast(m_Operator->GetCameraMode(), m_Operator->GetTankToPilot(), m_Operator->GetTankToPilot());
		break;
	}
	//We in theory shouldn't ever hit these two cases, so if we do then you need to follow the stack trace back to figure out how this was even possible.
	case ECameraMode::CM_ActionMode:
	{
		Possess(m_Operator->GetTankToPilot());
		m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
		break;
	}
	case ECameraMode::CM_SniperMode:
	{
		ToggleSniperHUD();
		Possess(m_Operator->GetTankToPilot());
		ToggleActionHUD();
		m_Operator->SetCameraMode(ECameraMode::CM_ActionMode);
		break;

	}
	default:
	{
		break;
	}
	}

}


