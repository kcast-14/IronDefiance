// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ProjectilePool.generated.h"




class AProjectileBase;
/**
 * 
 */
UCLASS()
class IRONDEFIANCE_API UProjectilePool : public UObject
{
	GENERATED_BODY()

public:

	UProjectilePool(const FObjectInitializer& Initializer);

	void Activate();

	void Deactivate(AProjectileBase* Proj);

private:
	AProjectileBase* GetProjectile();

private:

	TMap<AProjectileBase*, bool> m_Pool;
	
};
