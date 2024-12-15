// Fill out your copyright notice in the Description page of Project Settings.
/**
* Class Created by: Delano Wilcox
* This is a base class from which all characters will derive from.
*/


#include "Character/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/ProjectileBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AIController.h"
#include "InputActionValue.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.0f);
	
	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_SpringArmComponent->SetupAttachment(GetRootComponent());
	m_SpringArmComponent->SetRelativeLocation({ 0.f,0.f,80.f }); // {} are the equivalent of FVector()
	m_SpringArmComponent->SetRelativeRotation({ -60.f,0.f,0.f }); // Y,Z,X
	m_SpringArmComponent->TargetArmLength = 3000.f;
	m_SpringArmComponent->bInheritPitch = true;
	m_SpringArmComponent->bInheritYaw = true;
	m_SpringArmComponent->bInheritRoll = true;

	// Create a CameraComponent	
	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_CameraComponent->SetupAttachment(m_SpringArmComponent);
	m_CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	m_CameraComponent->FieldOfView = 90.f;
	m_CameraComponent->bUsePawnControlRotation = true;


	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	GetCharacterMovement()->MaxFlySpeed = 1000.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1000.f;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//Temp Because we're probably going to end up writing our own controller 
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (Controller)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(m_MappingContext, 0);
		}
	}
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		//I know we don't have jumping, but this is left here until I know for sure we don't need it.
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Triggered, this, &ACharacterBase::Pause);
		EnhancedInputComponent->BindAction(m_SwitchCameraModeAction, ETriggerEvent::Triggered, this, &ACharacterBase::SwitchCameraMode);
		EnhancedInputComponent->BindAction(m_ZoomAction, ETriggerEvent::Triggered, this, &ACharacterBase::Zoom);
		EnhancedInputComponent->BindAction(m_SelectAction, ETriggerEvent::Triggered, this, &ACharacterBase::Select); //Name is subject to change

	}

}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	//This should never be nullptr and if it is we have an issue
	check(Controller); 

	FVector2D MoveVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MoveVector.Y);
	AddMovementInput(GetActorRightVector(), MoveVector.X);
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	check(Controller);
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}


void ACharacterBase::Pause(const FInputActionValue& Value)
{
	//Call PlayerController Here and Display Pause Menu
}

void ACharacterBase::SwitchCameraMode(const FInputActionValue& Value)
{
	//Switch Camera Modes here
}

void ACharacterBase::Zoom(const FInputActionValue& Value)
{
	// Move Camera Closer

	float ZoomValue = Value.Get<float>();
	FVector CameraLocation = m_CameraComponent->GetRelativeLocation();
	CameraLocation.X *= ZoomValue;

	//Probably don't need this but including it anyway for later testing.
	m_CameraComponent->SetRelativeLocation(CameraLocation);
	

}

void ACharacterBase::Select(const FInputActionValue& Value)
{

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	float ModDamage = 0.f;
	ModDamage = CalculateDamage(DamageAmount, Cast<AProjectileBase>(DamageCauser));


	if (ModDamage <= 0.f)
	{
		if (m_CurrentHealth - DamageAmount <= 0.f)
		{
			m_CurrentHealth = 0.f;
			Die();

			if (DamageCauser)
			{
				//Do some stuff here with the thing that caused the damage
			}
		}
		else
		{
			m_CurrentHealth -= DamageAmount;
		}

		return DamageAmount;
	}
	else
	{
		if (m_CurrentHealth - ModDamage <= 0.f)
		{
			m_CurrentHealth = 0.f;
			Die();

			if (DamageCauser)
			{
				//Do some stuff here with the thing that caused the damage
			}
		}
		else
		{
			m_CurrentHealth -= ModDamage;
		}

		return ModDamage;
	}

}

float ACharacterBase::CalculateDamage(float BaseDamage, AProjectileBase* Projectile)
{
	// Returning 0 means either the Ammo Type was never set in BP or the object that caused the damage was not a projectile
	float DefModifier = 0.f;

	if (Projectile)
	{
		switch (Projectile->GetAmmoType())
		{
		case EAmmoType::AT_ArmorPiercing:
		{
			DefModifier = CalculateResistance();
			return BaseDamage * DefModifier;
			break;
		}
		case EAmmoType::AT_Apcr:
		{
			DefModifier = CalculateResistance();
			return BaseDamage * DefModifier;
			break;
		}
		case EAmmoType::AT_Heat:
		{
			DefModifier = CalculateResistance();
			return BaseDamage * DefModifier;
			break;
		}
		case EAmmoType::AT_Explosive:
		{
			DefModifier = CalculateResistance();
			return BaseDamage * DefModifier;
			break;
		}
		default:
		{
			DefModifier = 0.f;
			return DefModifier;
		}
		}
	}

	return 0.f;
}

float ACharacterBase::CalculateResistance()
{
	// All of these return values will be changed once we have an upgrade system implemented
	// From that point we'll basically take the Def modifier from the upgrade that the tank has and apply that using some algebraic formula and return whatever value that is.
	// The placeholder values are here for now just to be able to demonstrate that the system works.

	switch (m_TankType)
	{
	case ETankType::TT_M4Sherman:
	{
		return m_Stats.ArmorResistance * 1.1f;
		break;
	}
	case ETankType::TT_Tiger1:
	{
		return m_Stats.ArmorResistance * 1.2f;
		break;
	}
	case ETankType::TT_M10Wolverine:
	{
		return m_Stats.ArmorResistance * 1.3f;
		break;
	}
	default:
	{
		return 1.f;
	}
	}
}

void ACharacterBase::Fire()
{
	//Take care of some "thinking" code here
	//Fire;

	check(m_ProjectileClass); // All classes should have a projectile class set in BP

	FVector BarrelSocketLocation;
	FRotator BarrelSocketRotation;

	if (GetWorld()) // If the world exists
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		m_Projectile = GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, BarrelSocketLocation, BarrelSocketRotation);

		if (m_Projectile) // If the Projectile was successfully constructed and spawned
		{
			m_Projectile->SetInstigator(GetController());
			FVector LaunchDirection = BarrelSocketRotation.Vector();
			m_Projectile->FireInDirection(LaunchDirection);
			UGameplayStatics::SpawnEmitterAttached(m_Projectile->ProjectileParticles, m_Projectile->StaticMeshComponent, FName("ProjectileAttackSocket"), m_Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(1.f), EAttachLocation::SnapToTarget, true);
		}


	}
}

void ACharacterBase::Die()
{
	// This is kind of a cheap workaround since we only need to disable input if this particular player or tank is actively being controlled
	// by a player otherwise controller/player input doesn't really matter
	if (IsPlayerControlled())
	{
		DisableInput(GetLocalViewingPlayerController());
		//Do some dying stuff here
	}

}

bool ACharacterBase::IsDead()
{
	return false;
}

