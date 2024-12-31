// Fill out your copyright notice in the Description page of Project Settings.
/**
* Class Created by: Delano Wilcox
* This is a base class from which all projectiles will derive from. It contains all necessary functionality for a projectile to be fired as well as an implementation
* for what to do when the projectile hits something.
*/


#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/CharacterBase.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystem.h"
#include "AIController.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.f);
	RootComponent = CollisionComponent;
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);



	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = .05f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;

	m_Damage = 1.f;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StaticMeshComponent->SetWorldLocation(CollisionComponent->GetComponentLocation());

}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherActor && m_FireInstigator) // Delano: If The other actor is not this particular projectile AND OtherActor is valid AND FireInstigator is valid
	{
		// Delano: We're checking here for what it was that actually got hit, if it's not an enemy tank, or a player tank then we'll just assume it hit something in the world (it missed)
		ACharacterBase* Char = Cast<ACharacterBase>(OtherActor); 
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Char && m_FireInstigator != Char->GetController())
		{
			UGameplayStatics::ApplyDamage(Char, m_Damage, m_FireInstigator, this, m_DamageTypeClass);
			check(HitParticles);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Hit.ImpactPoint, FRotator(0.f), true);
			Destroy();
			return;
		}

		if (Enemy && m_FireInstigator != Enemy->GetController())
		{
			UGameplayStatics::ApplyDamage(Enemy, m_Damage, m_FireInstigator, this, m_DamageTypeClass);
			check(HitParticles);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Hit.ImpactPoint, FRotator(0.f), true);
			Destroy();
			return;
		}

		if(!Char && !Enemy)
		{
			check(HitWorldParticles);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorldParticles, Hit.ImpactPoint, FRotator(0.f), true);
			Destroy();
			return;
		}
	}
}

