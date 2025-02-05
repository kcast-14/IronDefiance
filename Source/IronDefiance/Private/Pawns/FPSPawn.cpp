// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/FPSPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AFPSPawn::AFPSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_SpringArmComponent->SetupAttachment(GetRootComponent());
	m_SpringArmComponent->TargetArmLength = 0.f;
	m_SpringArmComponent->bInheritPitch = true;
	m_SpringArmComponent->bInheritYaw = true;
	m_SpringArmComponent->bInheritRoll = true;

	// Create a CameraComponent	
	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_CameraComponent->SetupAttachment(m_SpringArmComponent);
	m_CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	m_CameraComponent->FieldOfView = 90.f;
	m_CameraComponent->bUsePawnControlRotation = true;

	m_MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
}

// Called when the game starts or when spawned
void AFPSPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

