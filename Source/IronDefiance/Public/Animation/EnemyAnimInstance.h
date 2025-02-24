// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
class APawn;
class AEnemy;

UCLASS()
class IRONDEFIANCE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//Functions
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "MovementSpeed"))
	float m_MovementSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Yaw Delta"))
	float m_YawDelta = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Rotation On Last Tick"))
	FRotator m_RotationLastTick = { 0.f,0.f,0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Pawn Pointer"))
	APawn* m_Pawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Enemy Pointer"))
	AEnemy* m_Enemy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Pitch (Y-Axis)"))
	float m_Pitch = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Roll (X-Axis)"))
	float m_Roll = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Yaw (Z-Axis)"))
	float m_Yaw = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Target Rotation"))
	float m_TargetRotation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Is In Air?"))
	bool bIsInAir = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Is Accelerating?"))
	bool bIsAccelerating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Is Attacking?"))
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Is FullBody?"))
	bool bIsFullBody = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Direction"))
	float m_Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Stop Direction"))
	float m_StopDirection;
	
};
