// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDEnums.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UParticleSystem;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class AAIController;
class AEnemy;
class ACharacterBase;

UCLASS()
class IRONDEFIANCE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionComponent;

	//Particles emitted while Projectile is moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UParticleSystem* ProjectileParticles;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UParticleSystem* HitWorldParticles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void FireInDirection(const FVector& ShootDirection);

	EAmmoType GetAmmoType() { return m_AmmoType; }

	//Called when Projectile Hit's Something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	FORCEINLINE void SetInstigator(AController* Inst) { m_FireInstigator = Inst; }

private:

	FVector m_Location;
	TArray<FVector> m_ProjectileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Instigator"))
	AController* m_FireInstigator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Base Damage"))
	float m_Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "AI Controller"))
	AAIController* m_AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Damage Type Class"))
	TSubclassOf<UDamageType> m_DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Ammo Type"))
	EAmmoType m_AmmoType = EAmmoType::DEFAULT_MAX;
};
