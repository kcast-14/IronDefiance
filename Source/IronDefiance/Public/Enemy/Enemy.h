#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IDEnums.h"
#include "IDStructs.h"
#include "Enemy.generated.h"

class ACharacterBase;
class AAIController;
class AProjectileBase;
class AWave;

UCLASS()
class IRONDEFIANCE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float CalculateDamage(float BaseDamage, AProjectileBase* Projectile);

	float CalculateResistance();

	void Fire();

	void Die();

	void SetWavePointer(AWave* Ptr);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "AI Controller"))
	AAIController* m_AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Combat Target"))
	ACharacterBase* m_CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Max Stat Values"))
	FTankStats m_Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Health"))
	float m_CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyInfo", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Pointer To Wave Class"))
	AWave* m_WavePtr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Projectile", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Projectile Class"))
	TSubclassOf<AProjectileBase> m_ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Has Valid Target?"))
	bool bHasValidTarget;

	AProjectileBase* m_Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|TankInfo", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tank Type"))
	ETankType m_TankType = ETankType::DEFAULT_MAX;
};
