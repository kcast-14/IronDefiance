// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IDGameModeBase.generated.h"

/**
 * 
 */

class AFOBActor;

UCLASS()
class IRONDEFIANCE_API AIDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	
	AIDGameModeBase();

	FORCEINLINE AFOBActor* GetFOBPointer() { return m_FOB; }

	FORCEINLINE void SetFOBPointer(AFOBActor* FOB) { m_FOB = FOB; }

private:

	AFOBActor* m_FOB;
};
