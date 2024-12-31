// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FOBActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDangerZoneEntered, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDangerZoneExited, AActor*, Target);

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class IRONDEFIANCE_API AFOBActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFOBActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_AgroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;


	UPROPERTY(BlueprintAssignable)
	FOnDangerZoneEntered m_OnDangerZoneEntered;

	UPROPERTY(BlueprintAssignable)
	FOnDangerZoneExited m_OnDangerZoneExited;

public:
	UFUNCTION()
	virtual void OnAgroOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAgroOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
