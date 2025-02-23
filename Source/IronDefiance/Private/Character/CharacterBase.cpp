// Fill out your copyright notice in the Description page of Project Settings.
/**
* Class Created by: Delano Wilcox
* This is a base class from which all characters will derive from.
*/


#include "Character/CharacterBase.h"
#include "Actors/FOBActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/IDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectiles/ProjectileBase.h"
#include "Controllers/IDPlayerController.h"
#include "AIController.h"




// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.0f);
	
	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_SpringArmComponent->SetupAttachment(GetRootComponent());
	m_SpringArmComponent->SetRelativeLocation({ 0.f,0.f,20.f }); // {} are the equivalent of FVector()
	m_SpringArmComponent->SetRelativeRotation({ 320.f,0.f,0.f }); // Y,Z,X
	m_SpringArmComponent->TargetArmLength = 200.f;
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
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	m_CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	m_CombatSphere->SetupAttachment(GetRootComponent());
	m_CombatSphere->InitSphereRadius(m_Stats.Range);

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	GetCharacterMovement()->MaxFlySpeed = 1000.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1000.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
	TMap<AFOBActor*, ETowerType> Towers = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetAllTowers();
	m_CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnCombatOverlapBegin);
	m_CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnCombatOverlapEnd);

	for (auto& T : Towers)
	{
		T.Key->m_OnDangerZoneEntered.AddDynamic(this, &ACharacterBase::OnEnemyEnteredDangerZone);
		T.Key->m_OnDangerZoneExited.AddDynamic(this, &ACharacterBase::OnEnemyExitedDangerZone);
	}
	m_CurrentHealth = m_Stats.MaxHealth;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CombatTarget)
	{
		Attack();
	}
	else
	{
		SwitchCombatTargets();
	}

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	FTransform SpawnTransform = GetMesh()->GetSocketTransform(TEXT("HeadSocket"));
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

	m_FPSPawn = GetWorld()->SpawnActor<AFPSPawn>(m_FPSCamPawn, SpawnTransform);

	GetMesh()->GetSocketByName("HeadSocket")->AttachActor(m_FPSPawn, GetMesh());
	
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

void ACharacterBase::Attack()
{
	if (CanHitTarget())
	{
		InterpToTarget();
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(m_FireTimerHandle, this, &ACharacterBase::Fire, (1.f/m_Stats.FireRate), false);
		}
	}
	else
	{
		SwitchCombatTargets();
	}
}

void ACharacterBase::Fire()
{
	//Take care of some "thinking" code here

	check(m_ProjectileClass); // All classes should have a projectile class set in BP

	FVector BarrelSocketLocation = GetActorLocation(); // Temp Values
	FRotator BarrelSocketRotation = GetActorRotation(); // Temp Values
	BarrelSocketLocation += FVector(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), 0.f, 0.f);

	if (GetWorld()) // If the world exists
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		m_Projectile = GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, BarrelSocketLocation, BarrelSocketRotation);
		m_Projectile->AddDamageUpgrade(m_Stats.DamageAddition);

		if (m_Projectile) // If the Projectile was successfully constructed and spawned
		{
			m_Projectile->SetInstigator(GetController());
			m_Projectile->CollisionComponent->BodyInstance.SetCollisionProfileName(FName("PlayerProjectiles"));
			FVector LaunchDirection = BarrelSocketRotation.Vector();
			m_Projectile->FireInDirection(LaunchDirection);
			//UGameplayStatics::SpawnEmitterAttached(m_Projectile->ProjectileParticles, m_Projectile->StaticMeshComponent, FName("ProjectileAttackSocket"), m_Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(1.f), EAttachLocation::SnapToTarget, true);
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

	m_OnTankDestoryed.Broadcast(this); 
	GetWorld()->GetTimerManager().SetTimer(m_DeathTimerHandle, this, &ACharacterBase::DestroyTank, 5.f, false);
	//Destroy();
}

void ACharacterBase::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		m_TargetsInRange.Add(Enemy);

		if (m_CombatTarget == nullptr)
		{
			m_CombatTarget = Enemy;
		}
	}
}

void ACharacterBase::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		RemoveCombatTarget(Enemy);

		if (m_CombatTarget == Enemy)
		{
			SwitchCombatTargets();
		}
	}

}

bool ACharacterBase::IsDead()
{
	return m_MovementStatus == EMovementStatus::MS_Dead;
}

void ACharacterBase::OnEnemyEnteredDangerZone(AActor* Target)
{
	AEnemy* Enemy = Cast<AEnemy>(Target);

	if (Enemy != nullptr)
	{
		if (IsTargetInRange(Enemy))
		{
			m_CombatTarget = Enemy;
			m_TargetsInDangerZone.Add(Enemy);
		}
	}
}

void ACharacterBase::OnEnemyExitedDangerZone(AActor* Target)
{
	AEnemy* Enemy = Cast<AEnemy>(Target);

	int32 Index = m_TargetsInRange.Find(Enemy);
	if (Index == -1)
	{
		return;
	}

	if (!m_TargetsInDangerZone.IsEmpty())
	{
		for (auto E : m_TargetsInDangerZone)
		{
			if (IsTargetInRange(E))
			{
				m_CombatTarget = E;
				return;
			}
		}

	}

}



void ACharacterBase::ApplyUpgrade(float Value, EUpgradeType Type)
{
	m_Stats.UpgradeStat(Value, Type);

	//TODO: Show some sort of visual indication of upgrade here
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_UpgradeParticles, GetActorTransform());
}

void ACharacterBase::RemoveCombatTarget(AEnemy* Enemy)
{
	int32 Index = m_TargetsInRange.Find(Enemy);

	m_TargetsInRange.RemoveAt(Index);
}

void ACharacterBase::SwitchCombatTargets()
{
	if (!m_TargetsInRange.IsEmpty())
	{
		//We do this in this manner because we don't exactly know what index will be valid next, so we find the first valid Enemy
		AEnemy** ValidEnemy = m_TargetsInRange.FindByPredicate([&](const AEnemy* Element)
			{
				if (IsTargetInRange(Element) && Element->GetMovementStatus() != EMovementStatus::MS_Dead)
				{
					return Element;
				}
				else
				{
					Element = nullptr;
					return Element;
				}
			});

		if (ValidEnemy != nullptr)
		{
			m_CombatTarget = *ValidEnemy;
		}
	}
	else
	{
		m_CombatTarget = nullptr;
	}
}

bool ACharacterBase::ShouldAttack(AEnemy* Enemy)
{
	if (Enemy != nullptr)
	{
		return false;
	}

	return false;
}

bool ACharacterBase::CanHitTarget()
{
	float CastLength = m_Stats.Range* 10.f;

	FHitResult Result;

	FVector StartLocation = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector Direction = GetActorForwardVector();

	FVector EndLocation = StartLocation + (Direction * CastLength);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	//CollisionParams.AddIgnoredComponent()
	if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_GameTraceChannel6, CollisionParams))
	{
		AEnemy* HitEnemy = Cast<AEnemy>(Result.GetActor());
		if (HitEnemy == m_CombatTarget)
		{
			return true;
		}
		if ( HitEnemy != nullptr) //TODO: Work with this some more to make it switch if the enemy it hit is closer than the current target
		{

			return true;
		}

	}

	return false;
}

bool ACharacterBase::IsTargetInRange(AEnemy* Enemy)
{
	FVector TankLocation = GetActorLocation();
	FVector EnemyLocation = Enemy->GetActorLocation();
	double Distance = FMath::Sqrt(
		FMath::Pow((EnemyLocation.X - TankLocation.X), 2.0) + 
		FMath::Pow((EnemyLocation.Y - TankLocation.Y), 2.0) + 
		FMath::Pow((EnemyLocation.Z - TankLocation.Z), 2.0)
	);

	return Distance <= m_Stats.Range;
}

bool ACharacterBase::IsTargetInRange(const AEnemy* Enemy)
{
	FVector TankLocation = GetActorLocation();
	FVector EnemyLocation = Enemy->GetActorLocation();
	double Distance = FMath::Sqrt(
		FMath::Pow((EnemyLocation.X - TankLocation.X), 2.0) +
		FMath::Pow((EnemyLocation.Y - TankLocation.Y), 2.0) +
		FMath::Pow((EnemyLocation.Z - TankLocation.Z), 2.0)
	);

	return Distance <= m_Stats.Range;
}

void ACharacterBase::InterpToTarget()
{
	//FRotator MuzzleYaw = GetMuzzleRotationYaw(m_CombatTarget->GetActorLocation(), FName("Muzzle_L"));
	FRotator MuzzleYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),m_CombatTarget->GetActorLocation());
	FRotator Rotation(0.f, MuzzleYaw.Yaw, 0.f);
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), 20.f);

	SetActorRotation(InterpRotation);
	return;
}

void ACharacterBase::DestroyTank()
{
	Destroy();
}

