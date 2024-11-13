// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTower.h"
#include "DrawDebugHelpers.h"
// Sets default values
ABaseTower::ABaseTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RotationRate = 45.f;
}

// Called when the game starts or when spawned
void ABaseTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Red, false, -1.f);
}

