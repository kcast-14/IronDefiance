// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "IDAIController.generated.h"

/**
 * 
 */
UCLASS()
class IRONDEFIANCE_API AIDAIController : public AAIController
{
	GENERATED_BODY()

public:
	AIDAIController(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

};
