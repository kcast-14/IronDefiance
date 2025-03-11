// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"
#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameInstance/IDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"

UProjectilePool::UProjectilePool(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
}

void UProjectilePool::Init(TSubclassOf<AProjectileBase>& ProjectileClass)
{
	Cast<UIDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->m_OnLevelSwitch.AddDynamic(this, &UProjectilePool::DeInit);
	m_Pool.Reserve(1000);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < 1000; i++)
	{
		//m_Pool.Emplace(NewObject<AProjectileBase>(this, ProjectileClass), false);
		m_Pool.Emplace(GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, FVector(0.f), FRotator(0.f), SpawnParams), false);
	}
}

void UProjectilePool::DeInit()
{
	m_Pool.Empty();
}

void UProjectilePool::Activate(AProjectileBase* Proj)
{
	m_Pool[Proj] = true;
	Proj->SetActorHiddenInGame(false);
	Proj->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (Proj->m_PoolMethod == EProjPoolMethod::AutoRelease)
	{
		Proj->BindFunction(std::bind(&UProjectilePool::Deactivate, this, std::placeholders::_1));
	}
	//GetWorld()->SpawnActor<AProjectileBase>(Proj->, FVector(0.f), FRotator(0.f));
	
}

void UProjectilePool::Deactivate(AProjectileBase* Proj)
{
	m_Pool[Proj] = false;
	Proj->SetActorHiddenInGame(true);
	Proj->m_ParticleSystemComponent->Deactivate();
	Proj->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Proj->m_PoolMethod == EProjPoolMethod::AutoRelease)
	{
		Proj->UnbindFunction();
	}
}

AProjectileBase* UProjectilePool::RequestProjectile(EProjPoolMethod ReleaseMethod)
{
	return GetProjectile(ReleaseMethod);
}

void UProjectilePool::BeginDestroy()
{
	for (auto& P : m_Pool)
	{
		P.Key->Destroy();
	}

	Super::BeginDestroy();
}

AProjectileBase* UProjectilePool::GetProjectile(EProjPoolMethod ReleaseMethod)
{
	AProjectileBase* Proj = FindAvailableProjectile();
	if (Proj == nullptr)
	{
		return nullptr;
	}
	Activate(Proj);
	Proj->m_PoolMethod = ReleaseMethod;

	return Proj;
}

AProjectileBase* UProjectilePool::FindAvailableProjectile()
{
	AProjectileBase* const* Proj = m_Pool.FindKey(false);
	if (Proj == nullptr)
	{
		return nullptr;
	}

	return *Proj;
}
