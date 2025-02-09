// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IDEnums.h"
#include "OperatorPawn.generated.h"

class ACharacterBase;
class UCameraComponent;
class USpringArmComponent;
class UFloatingPawnMovement;
class UStaticMeshComponent;

UCLASS()
class IRONDEFIANCE_API AOperatorPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOperatorPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* m_MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_MeshComponent;

	FORCEINLINE virtual UCameraComponent* GetCameraComponent() { return m_CameraComponent; }

	FORCEINLINE virtual USpringArmComponent* GetSpringArmComponent() { return m_SpringArmComponent; }

	FORCEINLINE virtual TSubclassOf<ACharacterBase> GetTankToPlace() { return m_TankToPlace; }

	FORCEINLINE virtual ACharacterBase* GetTankToPilot() { return m_TankToPilot; }

	FORCEINLINE virtual void SetTankToPilot(AActor* Tank);

	FORCEINLINE virtual bool IsPilotingTank() { return bIsPilotingTank; }

	FORCEINLINE virtual void CanPilotTank(bool Can) { bIsPilotingTank = Can; }

	FORCEINLINE virtual ECameraMode GetCameraMode() { return m_CameraMode; }
	FORCEINLINE virtual void SetCameraMode(ECameraMode Mode) { m_CameraMode = Mode; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual AActor* PlaceTank(FVector WorldLocation, FVector WorldDirection);


	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void SetTankToPlace(TSubclassOf<ACharacterBase> Tank) { m_TankToPlace = Tank; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Is Piloting a Tank?"))
	bool bIsPilotingTank = false;

	//Which type of Tank should be places
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Info", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tank To Place"))
	TSubclassOf<ACharacterBase> m_TankToPlace = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Info", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tank To Pilot"))
	ACharacterBase* m_TankToPilot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Info", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Camera Mode"))
	ECameraMode m_CameraMode = ECameraMode::DEFAULT_MAX;


};
