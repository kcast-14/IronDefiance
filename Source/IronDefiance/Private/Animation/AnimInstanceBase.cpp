// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstanceBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Controllers/IDPlayerController.h"
#include "Pawns/OperatorPawn.h"
#include "Character/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAnimInstanceBase::NativeInitializeAnimation()
{
}

void UAnimInstanceBase::NativeBeginPlay()
{
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
	}

	if (m_Pawn)
	{
		m_Tank = Cast<ACharacterBase>(m_Pawn);
		if (m_Tank)
		{

		}
	}

	m_PC = Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ensureMsgf(m_PC, TEXT("Player Controller was invalid"));

	m_PC->m_OnModeSwitch.AddDynamic(this, &UAnimInstanceBase::OnModeChanged);
	m_PC->m_OnRotate.AddDynamic(this, &UAnimInstanceBase::OnTankRotated);
}

void UAnimInstanceBase::UpdateAnimationProperties()
{

	
	if (m_Pawn == nullptr)
	{
		m_Pawn = TryGetPawnOwner();
		if (m_Pawn)
		{
			m_Tank = Cast<ACharacterBase>(m_Pawn);
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
		FRotator ActorRotation = {0.f, 0.f, 0.f,};
		FRotator ActorOriginRotation = { 0.f, 0.f, 0.f, };
		if (m_Tank != nullptr)
		{
			ActorOriginRotation = m_Tank->GetTankRotation();
		}
		if (m_CurrentMode == ECameraMode::CM_SniperMode)
		{
			ActorRotation = m_Tank->GetActorRotation();
		}
		else
		{
			ActorRotation = m_Pawn->GetActorRotation();
		}

		m_Roll = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Roll;
		m_Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Pitch;
		m_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Yaw;

		//Set Yaw Delta

		if (m_CurrentMode == ECameraMode::CM_SniperMode)
		{
			m_RotationLastTick = m_Tank->GetActorRotation();
		}
		else
		{
			m_RotationLastTick = m_Pawn->GetActorRotation();
		}

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
			//Set Acceleration boolean
			if (m_Tank)
			{
				float CurrentAcceleration = m_Tank->GetCharacterMovement()->GetCurrentAcceleration().Size();
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

void UAnimInstanceBase::OnModeChanged(ECameraMode Mode)
{
	m_CurrentMode = Mode;
}

void UAnimInstanceBase::OnTankRotated(float RotateVal)
{
	m_CurrentTankRotation.Pitch = RotateVal;
}
