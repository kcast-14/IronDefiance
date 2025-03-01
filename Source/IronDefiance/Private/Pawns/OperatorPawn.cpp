// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/OperatorPawn.h"
#include "Character/CharacterBase.h"
#include "Controllers/IDPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AOperatorPawn::AOperatorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	m_MeshComponent->SetupAttachment(GetRootComponent());

	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	m_SpringArmComponent->SetupAttachment(m_MeshComponent);
	m_SpringArmComponent->SetRelativeLocation({ -20.f,80.f,0.f }); // {} are the equivalent of FVector()
	m_SpringArmComponent->SetRelativeRotation({ 315.f,0.f,0.f }); // Y,Z,X
	m_SpringArmComponent->TargetArmLength = 100.f;
	m_SpringArmComponent->bInheritPitch = true;
	m_SpringArmComponent->bInheritYaw = true;
	m_SpringArmComponent->bInheritRoll = true;
	m_SpringArmComponent->bUsePawnControlRotation = false;

	// Create a CameraComponent	
	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Operator Camera"));
	m_CameraComponent->SetupAttachment(m_SpringArmComponent);
	m_CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	m_CameraComponent->SetRelativeRotation({0.f,0.f,0.f}); // Rotate the camera
	m_CameraComponent->FieldOfView = 90.f;
	m_CameraComponent->bUsePawnControlRotation = false;

	m_MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));

	m_CameraMode = ECameraMode::CM_TacticianMode;
}


// Called when the game starts or when spawned
void AOperatorPawn::BeginPlay()
{
	Super::BeginPlay();

	Cast<AIDPlayerController>(GetController())->SetOperatorPointer(this);
	
}

// Called every frame
void AOperatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOperatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOperatorPawn::SetTankToPilot(AActor* Tank)
{
	m_TankToPilot = Cast<ACharacterBase>(Tank);
}

AActor* AOperatorPawn::PlaceTank(FVector WorldLocation, FVector WorldDirection)
{
	FActorSpawnParameters SpawnParameters{};
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform TankTransform;
	TankTransform.SetLocation(WorldLocation);
	FRotator Rotation;
	
	//It should face forward by default but if it doesn't then we'll force it here
	ACharacterBase* PlacedTank = GetWorld()->SpawnActor<ACharacterBase>(m_TankToPlace, TankTransform);

	if (PlacedTank)
	{
		m_TankToPlace = nullptr;
		return PlacedTank;
	}

	return nullptr;
}

