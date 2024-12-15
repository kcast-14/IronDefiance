// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IDEnums.h"
#include "IDStructs.h"
#include "CharacterBase.generated.h"

class AAIController;
class AEnemy;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class AAIController;
class AProjectileBase;
struct FInputActionValue;

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

	//The following functions should ONLY be called if the Actor is being controlled by the player
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);
	void SwitchCameraMode(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float CalculateDamage(float BaseDamage, AProjectileBase* Projectile);
	float CalculateResistance();

	void Fire();
	void Die();

	bool IsDead();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_SpringArmComponent;


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	float m_CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	FTankStats m_Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* m_MappingContext;

	//Action Mapping Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_TurnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_SwitchCameraModeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* m_SelectAction;

	//Which type of Tank should be places
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	AActor* m_TankToPlace = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIController* m_AIController;

	AEnemy* m_CombatTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> m_ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bHasValidTarget;

	AProjectileBase* m_Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ETankType m_TankType = ETankType::DEFAULT_MAX;



};
