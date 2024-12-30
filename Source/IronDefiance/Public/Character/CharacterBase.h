// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pawns/FPSPawn.h"
#include "IDEnums.h"
#include "IDStructs.h"
#include "CharacterBase.generated.h"

class AAIController;
class AEnemy;
class AAIController;
class AProjectileBase;
class UCameraComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USphereComponent;
class USpringArmComponent;
class UParticleSystem;


UCLASS()
class IRONDEFIANCE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float CalculateDamage(float BaseDamage, AProjectileBase* Projectile);
	virtual float CalculateResistance();

	virtual void Fire();
	virtual void Die();

	UFUNCTION()
	virtual void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual bool IsDead();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyUpgrade(float Value, EUpgradeType Type = EUpgradeType::DEFAULT_MAX);

	//Inlines

	FORCEINLINE virtual UCameraComponent* GetCameraComponent() { return m_CameraComponent; }

	FORCEINLINE virtual USpringArmComponent* GetSpringArmComponent() { return m_SpringArmComponent; }

	FORCEINLINE virtual AFPSPawn* GetFPSPawn() { return m_FPSPawn; }

	FORCEINLINE virtual ECameraMode GetCameraMode() { return m_CameraMode; }

	FORCEINLINE virtual void SetCameraMode(ECameraMode Mode) { m_CameraMode = Mode; }

	FORCEINLINE virtual float GetCameraSpeed() { return m_CameraSpeed; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_CombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_SpringArmComponent;


private:

	void RemoveCombatTarget(AEnemy* Enemy);

	void SwitchCombatTargets();
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName="Max Stat Values"))
	FTankStats m_Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Health"))
	float m_CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Armor Piercing Rounds"))
	float m_CurrentAPRounds = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current APCR Rounds"))
	float m_CurrentApcrRounds = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Heat Rounds"))
	float m_CurrentHeatRounds = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Explosive Rounds"))
	float m_CurrentExplosiveRounds = 0.f;

	bool bOverlappingCombatSphere = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "AI Controller"))
	AAIController* m_AIController = nullptr;
	
	TArray<AEnemy*> m_TargetsInRange;

	AEnemy* m_CombatTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Projectile", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Projectile Class"))
	TSubclassOf<AProjectileBase> m_ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Has Valid Target?"))
	bool bHasValidTarget = false;

	AProjectileBase* m_Projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|TankInfo", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tank Type"))
	ETankType m_TankType = ETankType::DEFAULT_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Camera Mode"))
	ECameraMode m_CameraMode = ECameraMode::CM_ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Movement Status"))
	EMovementStatus m_MovementStatus = EMovementStatus::DEFAULT_MAX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FPSCamera", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "FPS Pawn Class"))
	TSubclassOf<AFPSPawn> m_FPSCamPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPSCamera", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Camera Speed"))
	float m_CameraSpeed = 10.f;

	UPROPERTY()
	AFPSPawn* m_FPSPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Upgrade Particles"))
	UParticleSystem* m_UpgradeParticles;


};
