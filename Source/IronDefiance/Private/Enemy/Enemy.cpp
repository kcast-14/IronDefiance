#include "Enemy/Enemy.h"
#include "Actors/Wave.h"
#include "Actors/FOBActor.h"
#include "Character/CharacterBase.h"
#include "Controllers/IDPlayerController.h"
#include "Controllers/IDAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInstance/IDGameInstance.h"
#include "GameModes/IDGameModeBase.h"
#include "Projectiles/ProjectileBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"


/**
* Brought to you in part by De'Lano Wilcox and Nath
*/

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	m_Stats.MaxHealth = 100.f;

	m_CurrentHealth = m_Stats.MaxHealth;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	m_CombatSphere->SetupAttachment(GetRootComponent());
	m_CombatSphere->InitSphereRadius(m_Stats.Range);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	m_AIController = Cast<AIDAIController>(GetController());
	m_AIController->SetOwningActor<AEnemy>(this);
	m_WavePtr = GetGameInstance<UIDGameInstance>()->GetWavePtr();
	Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->AddEnemyLocation(this, GetActorLocation());

	m_CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapBegin);
	m_CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapEnd);

	m_MovementStatus = EMovementStatus::MS_Idle;

	m_Target = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetFOBPointer();
	MoveToTarget(m_Target);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldAttack())
	{
		Attack();
	}
}

void AEnemy::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/**
* Added by Delano
*/
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AEnemy::Attack()
{
	if (CanHitTarget())
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(m_FireTimerHandle, this, &AEnemy::Fire, (1.f / m_Stats.FireRate), false);
		}

	}
	else
	{
		if (m_CombatTarget != nullptr)
		{
			if (!IsCombatTargetTooFar(m_CombatTarget)) //See if distance to target is less than acceptance radius
			{
				//Interpolate to Target Tank
				InterpToTarget();
			}
			else
			{
				MoveToTarget(m_CombatTarget);
			}
		}
		else
		{
			if (m_AIController->IsMoveComplete())
			{
				MoveToTarget(m_Target);
			}
		}

	}

}

/**
* Added by Delano
*/
float AEnemy::CalculateDamage(float BaseDamage, AProjectileBase* Projectile)
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

/**
* Added by Delano
*/
float AEnemy::CalculateResistance()
{
	// All of these return values will be changed once we have an upgrade system implemented
	// From that point we'll basically take the Def modifier from the upgrade that the tank has and apply that using some algebraic formula and return whatever value that is.
	// The placeholder values are here for now just to be able to demonstrate that the system works.

	switch (m_TankType)
	{
	case ETankType::TT_M4Sherman:
	{
		return 1.1f;
		break;
	}
	case ETankType::TT_Tiger1:
	{
		return 1.2f;
		break;
	}
	case ETankType::TT_M10Wolverine:
	{
		return 1.3f;
		break;
	}
	default:
	{
		return 1.f;
	}
	}
}

/**
* Added by Delano
*/
void AEnemy::Fire()
{
	//Take care of some "thinking" code here
	//Fire;

	check(m_ProjectileClass); // All classes should have a projectile class set in BP

	FVector BarrelSocketLocation = GetActorLocation();
	BarrelSocketLocation += FVector(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), 0.f, 0.f);
	FRotator BarrelSocketRotation = GetActorRotation();

	if (GetWorld()) // If the world exists
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		m_Projectile = GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, BarrelSocketLocation, BarrelSocketRotation);

		if (m_Projectile && m_CombatTarget) // If the Projectile was successfully constructed and spawned
		{
			InterpToTarget();
			m_Projectile->SetInstigator(GetController());
			m_Projectile->CollisionComponent->BodyInstance.SetCollisionProfileName(FName("EnemyProjectiles"));
			FVector LaunchDirection = BarrelSocketRotation.Vector();
			m_Projectile->FireInDirection(LaunchDirection);
			//UGameplayStatics::SpawnEmitterAttached(m_Projectile->ProjectileParticles, m_Projectile->StaticMeshComponent, FName("ProjectileAttackSocket"), m_Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(1.f), EAttachLocation::SnapToTarget, true);
		}


	}
}

/**
* Added by Delano
*/
void AEnemy::Die()
{

	// Delano: We want to crash here as ALL enemy classes should have a pointer to the wave class in them and it should be getting set after it's spawned
	check(m_WavePtr);
	m_WavePtr->OnEnemyDefeated();

	Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->RemoveEnemyLocation(this);
	Destroy();
}

/**
* Added by Delano
*/
void AEnemy::SetWavePointer(AWave* Ref)
{
	m_WavePtr = Ref;
}

bool AEnemy::IsPlayerBlockingPath()
{
	float CastLength = m_Stats.Range * 10.f;

	FHitResult Result;

	FVector StartLocation = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw = 0.f;
	FVector Direction = GetActorForwardVector();

	FVector EndLocation = StartLocation + (Direction * CastLength);

	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 5.f, 0U, 3.f);
	if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_GameTraceChannel5, CollisionParams))
	{
		if (Cast<ACharacterBase>(Result.GetActor()))
		{
			return true;
		}
	}

	return false;

}

bool AEnemy::CanNavigateAround()
{

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(m_Target);
	MoveRequest.SetAcceptanceRadius(m_AcceptanceRadius);
	MoveRequest.SetAllowPartialPath(false);

	FPathFindingQuery PathQuery;

	m_AIController->BuildPathfindingQuery(MoveRequest, PathQuery);
	if (PathQuery.PathInstanceToFill != nullptr)
	{
		return true;
	}

	return false;
}

void AEnemy::PrepareToAttack()
{
	bShouldAttack = true;
	SetMovementStatus(EMovementStatus::MS_Attacking);
}

//This version of the function is just used to resume the movement towards the FOB
void AEnemy::MoveToTarget()
{
	InterpToGoal();
	MoveToTarget(Cast<AActor>(m_Target));
}

void AEnemy::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* Enemy = Cast<ACharacterBase>(OtherActor);
	if (Enemy)
	{
		if (m_CombatTarget == nullptr)
		{
			SetCombatTarget(Enemy);
		}
		//We could potentially add some logic here for the tank to try and choose between a target, or run (Fight or Flight) if possible.
	}
}

void AEnemy::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacterBase* Enemy = Cast<ACharacterBase>(OtherActor);

	if (Enemy && Enemy == m_CombatTarget)
	{
		m_CombatTarget->m_OnTankDestoryed.RemoveDynamic(this, &AEnemy::OnTargetDestroyed);
		m_CombatTarget = nullptr;
	}
}

void AEnemy::MoveToTarget(AActor* Target)
{
	SetMovementStatus(EMovementStatus::MS_MoveToTarget);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(m_AcceptanceRadius);
	MoveRequest.SetUsePathfinding(true);
	
	

	FNavPathSharedPtr NavPath;
	m_CurrentMoveRequest = MoveRequest;
	FPathFollowingRequestResult Result;

	Result = m_AIController->MoveTo(MoveRequest, &NavPath);


	if (NavPath != nullptr && Result.Code.GetValue() != EPathFollowingRequestResult::Failed) //If it's partial then there's probably something blocking it's path
	{
		m_AIController->SetIsPathPartial(NavPath.Get()->IsPartial());
		m_AIController->SetIsMoveComplete(false);

		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();

		for (FNavPathPoint& Points : PathPoints)
		{
			FVector& Location = Points.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
	}



}

bool AEnemy::ShouldAttack()
{
	return bShouldAttack;
}

bool AEnemy::CanHitTarget()
{
	float CastLength = m_Stats.Range * 10.f;

	FHitResult Result;

	FVector StartLocation = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector Direction = GetActorForwardVector();

	FVector EndLocation = StartLocation + (Direction * CastLength);

	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 5.f, 0U, 3.f);
	if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_GameTraceChannel5, CollisionParams))
	{
		if (Cast<ACharacterBase>(Result.GetActor()))
		{
			if (Cast<ACharacterBase>(Result.GetActor()) != m_CombatTarget)
			{
				SetCombatTarget(Cast<ACharacterBase>(Result.GetActor()));
				return true;
			}

			return true;
		}
	}

	return false;
}

bool AEnemy::IsCombatTargetTooFar(ACharacterBase* Enemy)
{
	FVector TankLocation = GetActorLocation();
	FVector EnemyLocation = Enemy->GetActorLocation();
	double Distance = FMath::Sqrt(
		FMath::Pow((EnemyLocation.X - TankLocation.X), 2.0) +
		FMath::Pow((EnemyLocation.Y - TankLocation.Y), 2.0) +
		FMath::Pow((EnemyLocation.Z - TankLocation.Z), 2.0)
	);

	return Distance > m_AcceptanceRadius;
}

void AEnemy::InterpToTarget()
{
	//FRotator MuzzleYaw = GetMuzzleRotationYaw(m_CombatTarget->GetActorLocation(), FName("Muzzle_L"));
	FRotator MuzzleYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_CombatTarget->GetActorLocation());
	FRotator Rotation(0.f, MuzzleYaw.Yaw, 0.f);
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), 20.f);

	SetActorRotation(InterpRotation);
	return;
}

//TODO: Temp
void AEnemy::InterpToGoal()
{
	//FRotator MuzzleYaw = GetMuzzleRotationYaw(m_CombatTarget->GetActorLocation(), FName("Muzzle_L"));
	FRotator MuzzleYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_Target->GetActorLocation());
	FRotator Rotation(0.f, MuzzleYaw.Yaw, 0.f);
	FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), 20.f);

	SetActorRotation(InterpRotation);
	return;
}

void AEnemy::SetCombatTarget(ACharacterBase* Enemy)
{
	m_CombatTarget = Enemy;
	//Cheesey workaround until I can figure out how to do this properly
	m_CombatTarget->m_OnTankDestoryed.RemoveDynamic(this, &AEnemy::OnTargetDestroyed);
	m_CombatTarget->m_OnTankDestoryed.AddDynamic(this, &AEnemy::OnTargetDestroyed);
}

void AEnemy::OnTargetDestroyed(ACharacterBase* Enemy)
{
	if (m_CombatTarget == Enemy)
	{
		m_CombatTarget->m_OnTankDestoryed.RemoveDynamic(this, &AEnemy::OnTargetDestroyed);
		m_CombatTarget = nullptr;
		bShouldAttack = false;
		MoveToTarget();
	}
}


