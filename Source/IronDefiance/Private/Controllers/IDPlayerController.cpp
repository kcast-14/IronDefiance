// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/IDPlayerController.h"
#include "Actors/Wave.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Enemy.h"
#include "GameInstance/IDGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pawns/OperatorPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

void AIDPlayerController::DisplayHUDOverlay()
{
	if (HUDOverlay)
	{
		bHUDOverlayVisible = true;
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AIDPlayerController::RemoveHUDOverlay()
{
	if (HUDOverlay)
	{
		bHUDOverlayVisible = false;
		HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

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
		bWaveTransitionVisible = true;
		m_WaveTransition->SetVisibility(ESlateVisibility::Hidden);
		IgnoreLookInput = false;
		IgnoreMoveInput = false;

		bShowMouseCursor = false;
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
		//if (!bLoadGameScreenVisible && !bSaveGameMenuVisible)
		//{
		//	IgnoreLookInput = false;
		//	IgnoreMoveInput = false;
		//	SetInputMode(InputModeGameOnly);
		//	bShowMouseCursor = false;
		//}

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
	}
}

void AIDPlayerController::ToggleHUDOverlay()
{
	if (bHUDOverlayVisible)
	{
		RemoveHUDOverlay();
	}
	else
	{
		DisplayHUDOverlay();
	}
}

void AIDPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
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
	if (m_CurrentControlledTank < m_Tanks.Num() )
	{
		m_CurrentControlledTank++;
		m_Operator->SetTankToPilot(m_Tanks[m_CurrentControlledTank]);
		return;
	}

	m_CurrentControlledTank = 0;
	m_Operator->SetTankToPilot(m_Tanks[m_CurrentControlledTank]);
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

	check(HUDOverlayAsset);
	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Hidden);


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

	/**
	* We'll uncomment everything below once we have more direction on what will actually be needed and not needed
	*/

	//check(WMainMenu);
	//FString MapName = GetWorld()->GetMapName();
	//MainMenu = CreateWidget<UUserWidget>(this, WMainMenu);
	//check(MainMenu)
	//MainMenu->AddToViewport();
	//MainMenu->SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(m_PlayerMappingContext, 0);
	}



	//See Wave.cpp: BeginPlay() for explaination on this
	if (GetGameInstance<UIDGameInstance>()->GetWavePtr() != nullptr)
	{
#if !UE_BUILD_SHIPPING
		DisplayWaveTransition();
		GetGameInstance<UIDGameInstance>()->GetWavePtr()->EnterTransition();
#endif

		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = true;
		ToggleOperatorHUD();
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
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Triggered, this, &AIDPlayerController::PauseGame);
		EnhancedInputComponent->BindAction(m_SwitchTank, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchTanks);
		EnhancedInputComponent->BindAction(m_SwitchToOperator, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchToOperator);
		EnhancedInputComponent->BindAction(m_SwitchCameraModeAction, ETriggerEvent::Triggered, this, &AIDPlayerController::SwitchCameraMode);
		EnhancedInputComponent->BindAction(m_ZoomAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Zoom);
		EnhancedInputComponent->BindAction(m_SelectAction, ETriggerEvent::Triggered, this, &AIDPlayerController::Select); //Name is subject to change

		//m_PlayerMappingContext->GetMappings()[0].

	}
}

bool AIDPlayerController::Raycast()
{
	//TODO: Change this to function in a way that actually checks for something rather than just passing around booleans
	return false;
}

AActor* AIDPlayerController::Raycast(FVector StartLocation, FVector Direction)
{
	float CastLength = 10000.f;

	FHitResult Result;

	FVector Location;
	FRotator Rotation;

	FVector EndLocation = StartLocation + (Direction * CastLength);
	
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		return Result.GetActor();
	}

	return nullptr;
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

	if (!m_Operator->IsPilotingTank())
	{
		m_Operator->AddMovementInput(m_Operator->GetActorForwardVector(), MoveVector.Y);
		m_Operator->AddMovementInput(m_Operator->GetActorRightVector(), MoveVector.X);
		return;
	}

	switch (m_Operator->GetTankToPilot()->GetCameraMode())
	{
	case ECameraMode::CM_ThirdPerson:
	{
		m_Operator->GetTankToPilot()->AddMovementInput(m_Operator->GetTankToPilot()->GetActorForwardVector(), (MoveVector.Y * m_Operator->GetTankToPilot()->GetCharacterMovement()->MaxWalkSpeed));
		m_Operator->GetTankToPilot()->AddMovementInput(m_Operator->GetTankToPilot()->GetActorRightVector(), (MoveVector.X * m_Operator->GetTankToPilot()->GetCharacterMovement()->MaxWalkSpeed));
		break;
	}
	case ECameraMode::CM_FirstPerson:
		//We probably need to do something with a camera here but maybe not
	{
		FVector Location = m_Operator->GetTankToPilot()->GetCameraComponent()->GetComponentTransform().GetLocation();

		Location.X += (MoveVector.X * m_Operator->GetTankToPilot()->GetCameraSpeed());
		Location.Y += (MoveVector.Y * m_Operator->GetTankToPilot()->GetCameraSpeed());
		break;
	}
	default:
	{
		break;
	}
	}


}

void AIDPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	
	if (m_Operator->IsPilotingTank())
	{
		switch (m_Operator->GetTankToPilot()->GetCameraMode())
		{
		case ECameraMode::CM_ThirdPerson:
		{
			FRotator Rotation = m_Operator->GetTankToPilot()->GetSpringArmComponent()->GetComponentRotation();
			Rotation.Yaw += LookVector.X;
			Rotation.Pitch += LookVector.Y;
			m_Operator->GetTankToPilot()->GetSpringArmComponent()->SetWorldRotation(Rotation);
			return;
		}
		case ECameraMode::CM_FirstPerson:
		{
			FRotator Rotation = GetPawn()->GetActorRotation();
			Rotation.Yaw += LookVector.X;
			Rotation.Pitch += LookVector.Y;
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

void AIDPlayerController::PauseGame(const FInputActionValue& Value)
{
	TogglePauseMenu();
}

void AIDPlayerController::SwitchCameraMode(const FInputActionValue& Value)
{
	if (!m_Operator->IsPilotingTank())
	{
		return;
	}

	switch (m_Operator->GetTankToPilot()->GetCameraMode())
	{
	case ECameraMode::CM_ThirdPerson:
	{
		RemoveHUDOverlay();
		RemoveOperatorHUD();
		m_Operator->GetTankToPilot()->GetFPSPawn()->SetActorLocation(m_Operator->GetTankToPilot()->GetActorLocation());
		Possess(m_Operator->GetTankToPilot()->GetFPSPawn());
		m_Operator->GetTankToPilot()->SetCameraMode(ECameraMode::CM_FirstPerson);
		ToggleSniperHUD();
		break;
	}
	case ECameraMode::CM_FirstPerson:
	{
		DisplayHUDOverlay();
		m_Operator->GetTankToPilot()->SetActorLocation(m_Operator->GetTankToPilot()->GetFPSPawn()->GetActorLocation());
		Possess(m_Operator->GetTankToPilot());
		m_Operator->GetTankToPilot()->SetCameraMode(ECameraMode::CM_ThirdPerson);
		ToggleSniperHUD();
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
	if (m_Operator->IsPilotingTank())
	{
		ToggleOperatorHUD();
		Possess(m_Operator);
		m_Operator->CanPilotTank(false);
		m_Operator->SetTankToPilot(nullptr);
		//m_Operator->GetSpringArmComponent()->SetRelativeRotation({ 315.f,0.f,0.f });
		m_CurrentControlledTank = -1;
	}
}

void AIDPlayerController::Zoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();

	if (!m_Operator->IsPilotingTank())
	{
		m_Operator->GetSpringArmComponent()->TargetArmLength += (ZoomValue * -50.f);
		return;
	}

	switch (m_Operator->GetTankToPilot()->GetCameraMode())
	{
	case ECameraMode::CM_ThirdPerson:
	{
		m_Operator->GetTankToPilot()->GetSpringArmComponent()->TargetArmLength += (ZoomValue * -50.f);
		break;
	}
	case ECameraMode::CM_FirstPerson:
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
		AActor* HitActor = Raycast(WorldLocation, WorldDirection);
		bool DidHit = IsHittingTankOrEnemy(HitActor);

		if (m_Operator->GetTankToPlace() != nullptr)
		{
			if (DidHit)
			{
				return;
			}
			else
			{
				PlaceTank(WorldLocation, WorldDirection);
			}
		}
		else
		{
			if (IsHittingAnotherTank(HitActor))
			{
				ToggleOperatorHUD();
				m_Operator->CanPilotTank(true);
				m_Operator->SetTankToPilot(HitActor);
				m_CurrentControlledTank = m_Tanks.Find(Cast<ACharacterBase>(HitActor)); // It should be noted that if you simply place tanks in the world this isn't going to work.
				Possess(Cast<APawn>(HitActor));
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


