#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IDEnums.h"
#include "IDStructs.h"
#include "AITypes.h"
#include "Enemy.generated.h"

class ACharacterBase;
class AFOBActor;
class AIDAIController;
class AProjectileBase;
class AWave;
class USphereComponent;
//Why wouldn't it let me forward declare a struct???

UCLASS()
class IRONDEFIANCE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Attack();

	void SetWavePointer(AWave* Ptr);

	void SetMovementStatus(EMovementStatus Status) { m_MovementStatus = Status; }

	bool IsPlayerBlockingPath();

	bool CanNavigateAround();

	void PrepareToAttack();

	void MoveToTarget();

	UFUNCTION()
	virtual void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTargetDestroyed(ACharacterBase* Enemy);


	FORCEINLINE ETankType GetTankType() { return m_TankType;}

	FORCEINLINE EMovementStatus GetMovementStatus() { return m_MovementStatus; }
	FORCEINLINE EMovementStatus GetMovementStatus() const { return m_MovementStatus; }

private:

	float CalculateDamage(float BaseDamage, AProjectileBase* Projectile);

	float CalculateResistance();

	void Fire();
	
	void Die();

	void MoveToTarget(AActor* Target);

	bool ShouldAttack();

	bool CanHitTarget();

	bool IsCombatTargetTooFar(ACharacterBase* Enemy);

	void InterpToTarget();

	void InterpToGoal();

	void SetCombatTarget(ACharacterBase* Enemy);



protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "AI Controller"))
	AIDAIController* m_AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Combat Target"))
	ACharacterBase* m_CombatTarget;

	bool bShouldAttack = false;


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

	FTimerHandle m_FireTimerHandle;



	AProjectileBase* m_Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|TankInfo", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tank Type"))
	ETankType m_TankType = ETankType::DEFAULT_MAX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Movement Status"))
	EMovementStatus m_MovementStatus = EMovementStatus::DEFAULT_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Acceptance Radius"))
	float m_AcceptanceRadius = 10.f;

	AFOBActor* m_Target;

	FAIMoveRequest m_CurrentMoveRequest;
};
