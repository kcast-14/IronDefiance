#include "Enemy/Enemy.h"
#include "TimerManager.h"
#include "GameModes/Wave.h"
#include "AIController.h"
#include "Projectiles/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	m_AIController = Cast<AAIController>(GetController());

	m_Stats.MaxHealth = 100.f;

	m_CurrentHealth = m_Stats.MaxHealth;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}

/**
* Added by Delano
*/
void AEnemy::SetWavePointer(AWave* Ref)
{
	m_WavePtr = Ref;
}


