// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"
#include "Projectiles/ProjectileBase.h"
#include "particles/ParticleSystem.h"

UProjectilePool::UProjectilePool(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
	m_Pool.Reserve(1000);

	for (int i = 0; i < 1000; i++)
	{
		m_Pool.Emplace({}, false);
	}
}

void UProjectilePool::Activate()
{

}

void UProjectilePool::Deactivate(AProjectileBase* Proj)
{
	m_Pool[Proj] = false;
	Proj->SetActorHiddenInGame(true);
	//Proj->HitParticles->
}

AProjectileBase* UProjectilePool::GetProjectile()
{
	return nullptr;
}
