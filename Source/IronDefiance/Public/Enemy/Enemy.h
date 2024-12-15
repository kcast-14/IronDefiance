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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIController* m_AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ACharacterBase* m_CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats", meta = (AllowPrivateAccess = "true"))
	FTankStats m_Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats", meta = (AllowPrivateAccess = "true"))
	float m_CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyInfo", meta = (AllowPrivateAccess = "true"))
	AWave* m_WavePtr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> m_ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bHasValidTarget;

	AProjectileBase* m_Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ETankType m_TankType = ETankType::DEFAULT_MAX;
};
