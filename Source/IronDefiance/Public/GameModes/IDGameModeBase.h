// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IDEnums.h"
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

	TArray<AFOBActor*> GetCrownTowers();
	TArray<AFOBActor*> GetEnergyTowers();
	TArray<AFOBActor*> GetCommunicationsTowers();

	FORCEINLINE TMap<ETowerType, AFOBActor*>& GetAllTowers() { return m_Towers; }
	FORCEINLINE uint32 GetCurrentNumCrowns() { return m_Crowns; }
	FORCEINLINE uint32 GetCurrentNumScraps() { return m_Scraps; }
	FORCEINLINE float GetCurrentAmountEnergy() { return m_UltimateProgress; }

	FORCEINLINE void SetCurrentNumCrowns(uint32 Value) { m_Crowns = Value; }
	FORCEINLINE void SetCurrentNumScraps(uint32 Value) { m_Scraps = Value; }
	FORCEINLINE void SetCurrentAmountEnergy(float Value) { m_UltimateProgress = Value; }

	void AddTowerPointer(AFOBActor* FOB);

	void IncrementCrowns(uint32 Value);
	void IncrementScrap(uint32 Value);
	void IncrementEnergy(float Value);

private:

	TMap<ETowerType, AFOBActor*> m_Towers;

	float m_UltimateProgress = 0.f;

	uint32 m_Crowns = 1000;

	uint32 m_Scraps = 0;
};
