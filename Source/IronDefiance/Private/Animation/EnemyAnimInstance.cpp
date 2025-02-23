// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
	}

	if (m_Pawn)
	{
		m_Enemy = Cast<AEnemy>(m_Pawn);
		if (m_Enemy)
		{
		}

	}
}

void UEnemyAnimInstance::NativeBeginPlay()
{
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
		if (m_Pawn)
		{
			m_Enemy = Cast<AEnemy>(m_Pawn);
		}
	}

	if (m_Pawn)
	{
		UWorld* World = GetWorld();
		//Set Speed
		FVector Velocity = m_Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Velocity.X, Velocity.Y, 0.f);
		m_MovementSpeed = LateralSpeed.Size();

		//Set bIsInAir
		bIsInAir = m_Pawn->GetMovementComponent()->IsFalling();

		//Set Roll, Pitch, and Yaw
		FRotator AimRotation = m_Pawn->GetBaseAimRotation();
		FRotator ActorRotation = m_Pawn->GetActorRotation();
		m_Roll = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Roll;
		m_Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Pitch;
		m_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Yaw;

		//Set Yaw Delta
		m_RotationLastTick = m_Pawn->GetActorRotation();
		m_TargetRotation = (UKismetMathLibrary::NormalizedDeltaRotator(m_RotationLastTick, ActorRotation).Yaw / World->GetDeltaSeconds()) / 10;
		m_YawDelta = FMath::FInterpTo(m_YawDelta, m_TargetRotation, World->GetDeltaSeconds(), 6.f);

		//Set bisFullBody
		float CurveValue = GetCurveValue("FullBody");
		if (CurveValue > 0)
		{
			bIsFullBody = true;
		}


		if (m_Pawn)
		{
			m_Enemy = Cast<AEnemy>(m_Pawn);

			//Set Acceleration boolean
			if (m_Enemy)
			{
				float CurrentAcceleration = m_Enemy->GetCharacterMovement()->GetCurrentAcceleration().Size();
				if (CurrentAcceleration > 0)
				{
					bIsAccelerating = true;
				}
				else
				{
					bIsAccelerating = false;
				}

			}

		}
	}
}
