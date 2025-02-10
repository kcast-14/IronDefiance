// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDEnums.h"
#include "FOBActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDangerZoneEntered, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDangerZoneExited, AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseDestroyed, AActor*, Target);

class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class IRONDEFIANCE_API AFOBActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFOBActor();

	UFUNCTION()
	virtual void OnAgroOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAgroOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool DecrementHealth(float Damage);

	UFUNCTION()
	void Die();

	UFUNCTION()
	FORCEINLINE ETowerType GetTowerType() { return m_Type; }

	UFUNCTION()
	FORCEINLINE void SetTowerType(ETowerType Type) { m_Type = Type; }


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* m_CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_AgroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;


	UPROPERTY(BlueprintAssignable)
	FOnDangerZoneEntered m_OnDangerZoneEntered;

	UPROPERTY(BlueprintAssignable)
	FOnDangerZoneExited m_OnDangerZoneExited;

	UPROPERTY(BlueprintAssignable)
	FOnBaseDestroyed m_OnBaseDestroyed;

	FTimerHandle m_Timer;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FORCEINLINE float GetHealth() {return m_Health;}
	FORCEINLINE float GetDelay() {return m_Delay;}
	FORCEINLINE float GetEnergyRate() {return m_EnergyToAdd;}
	FORCEINLINE uint32 GetCrownsToAdd() { return m_CrownsToAdd; }

	FORCEINLINE void SetHealth(float Value) { m_Health = Value; }
	FORCEINLINE void SetDelay(float Value) {m_Delay = Value;}
	FORCEINLINE void SetEnergyRate(float Value) {m_EnergyToAdd = Value;}
	FORCEINLINE void SetCrownsToAdd(uint32 Value) { m_CrownsToAdd = Value; }

	UFUNCTION()
	void AddCrowns();

	UFUNCTION()
	void IncrementUltimate();


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Base Health"))
	float m_Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Tower Type"))
	ETowerType m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Timer Delay"))
	float m_Delay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Number Of Crowns to Add"), meta = (ClampMin = "0"))
	int32 m_CrownsToAdd = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Amount of Energy to Add"))
	float m_EnergyToAdd = .01f;
};
