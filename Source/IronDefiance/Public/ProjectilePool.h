// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDEnums.h"
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

	UFUNCTION()
	void Init(TSubclassOf<AProjectileBase>& ProjectileClass);

	UFUNCTION()
	void DeInit();
	UFUNCTION()
	void Activate(AProjectileBase* Proj);
	UFUNCTION()
	void Deactivate(AProjectileBase* Proj);
	UFUNCTION()
	AProjectileBase* RequestProjectile(EProjPoolMethod ReleaseMethod = EProjPoolMethod::AutoRelease);
	virtual void BeginDestroy() override;

private:

	UFUNCTION()
	AProjectileBase* GetProjectile(EProjPoolMethod ReleaseMethod);
	UFUNCTION()
	AProjectileBase* FindAvailableProjectile();

private:

	// Represents a projectile and whether or not that projectile is activated and in use
	UPROPERTY(VisibleAnywhere)
	TMap<AProjectileBase*, bool> m_Pool;
	
};
