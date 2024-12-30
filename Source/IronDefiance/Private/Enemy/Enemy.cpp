#include "Enemy/Enemy.h"
#include "Character/CharacterBase.h"
#include "TimerManager.h"
#include "Controllers/IDPlayerController.h"
#include "Controllers/IDAIController.h"
#include "Components/SphereComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameInstance/IDGameInstance.h"
#include "Actors/Wave.h"
#include "Projectiles/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"



/**
* Brought to you in part by De'Lano Wilcox and Nath
*/

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	m_AIController = Cast<AIDAIController>(GetController());

	m_Stats.MaxHealth = 100.f;

	m_CurrentHealth = m_Stats.MaxHealth;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	m_CombatSphere->SetupAttachment(GetRootComponent());
	m_CombatSphere->InitSphereRadius(m_Stats.Range);
}

void AEnemy::MoveToTarget(AActor* Target)
{
	SetMovementStatus(EMovementStatus::MS_MoveToTarget);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(m_AcceptanceRadius);

	FNavPathSharedPtr NavPath;
	m_CurrentMoveRequest = MoveRequest;
	FPathFollowingRequestResult Result;

	Result = m_AIController->MoveTo(MoveRequest, &NavPath);

	/** Here we'll probably do something with the request result, or perhaps we'll have to make a custom PathFollowingComponent in order to account for this:
	* Allow for player-placed tanks to block paths partially, redirecting enemies to follow different routes. 
	* Implement logic that prompts enemies to attack the blocking tank if it completely obstructs their path.
	*/

	if (NavPath.Get()->IsPartial()) //If it's partial then there's probably something blocking it's path
	{
		//Figure out what's blocking it's path, then either attack or try to go around depending on whether or not it can make a path around.
	}

}

bool AEnemy::ShouldAttack()
{
	return false;

	//if (bPathCompletelyBlocked)
	//{
	//	return true;
	//}
}

bool AEnemy::CanHitTarget()
{
	float CastLength = m_Stats.Range;

	FHitResult Result;

	FVector StartLocation;
	FRotator Rotation;
	FVector Direction = Rotation.Vector();

	FVector EndLocation = StartLocation + (Direction * CastLength);

	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		if (Cast<ACharacterBase>(Result.GetActor()))
		{
			return true;
		}
	}

	return false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	m_WavePtr = GetGameInstance<UIDGameInstance>()->GetWavePtr();
	Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->AddEnemyLocation(this, GetActorLocation());

	m_CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapBegin);
	m_CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapEnd);

	m_MovementStatus = EMovementStatus::MS_Idle;

	check(m_TargetActor);
	m_Target = Cast<AActor>(m_TargetActor->GetDefaultObject());
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
		Fire();
	}
	else
	{
		if (false) //See if distance to target is less than acceptance radius
		{
			//Interpolate to Target Tank
		}
		else
		{
			//Move To Tank
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

/**
* Added by Delano
*/
void AEnemy::Die()
{
	Destroy();

	// Delano: We want to crash here as ALL enemy classes should have a pointer to the wave class in them and it should be getting set after it's spawned
	check(m_WavePtr);
	m_WavePtr->OnEnemyDefeated();

	Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->RemoveEnemyLocation(this);
}

/**
* Added by Delano
*/
void AEnemy::SetWavePointer(AWave* Ref)
{
	m_WavePtr = Ref;
}

void AEnemy::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* Enemy = Cast<ACharacterBase>(OtherActor);
	if (Enemy)
	{
		if (m_CombatTarget == nullptr)
		{
			m_CombatTarget = Enemy;
		}
		//We could potentially add some logic here for the tank to try and choose between a target, or run (Fight or Flight) if possible.
	}
}

void AEnemy::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacterBase* Enemy = Cast<ACharacterBase>(OtherActor);

	if (Enemy)
	{
		m_CombatTarget = nullptr;
	}
}


