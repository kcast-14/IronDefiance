// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundClass.h"
#include "IDEnums.h"
#include "IDGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelWon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLost);

class AFOBActor;

UCLASS()
class IRONDEFIANCE_API AIDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	
	AIDGameModeBase();

	virtual void BeginPlay() override;

	FORCEINLINE USoundClass* GetMaster() { return m_MasterSoundClass; }
	FORCEINLINE USoundClass* GetMusic() { return m_MusicSoundClass; }
	FORCEINLINE USoundClass* GetSFX() { return m_SFXSoundClass; }

	TArray<AFOBActor*> GetCrownTowers();
	TArray<AFOBActor*> GetEnergyTowers();
	TArray<AFOBActor*> GetCommunicationsTowers();

	FORCEINLINE TMap<AFOBActor*, ETowerType>& GetAllTowers() { return m_Towers; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentNumCrowns() { return m_Crowns; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentNumScraps() { return m_Scraps; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentAmountEnergy() { return m_UltimateProgress; }

	FORCEINLINE void SetCurrentNumCrowns(uint32 Value) { m_Crowns = Value; }
	FORCEINLINE void SetCurrentNumScraps(uint32 Value) { m_Scraps = Value; }
	FORCEINLINE void SetCurrentAmountEnergy(float Value) { m_UltimateProgress = Value; }

	void AddTowerPointer(AFOBActor* FOB);

	void RemoveTowerPointer(AFOBActor* FOB);

	void IncrementCrowns(uint32 Value);
	void IncrementScrap(uint32 Value);
	void IncrementEnergy(float Value);

	void WinGame();
	void LoseGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sound", meta=(DisplayName="Master"))
	USoundClass* m_MasterSoundClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sound", meta=(DisplayName="Music"))
	USoundClass* m_MusicSoundClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sound", meta=(DisplayName="Sound Effects"))
	USoundClass* m_SFXSoundClass;

	UPROPERTY(BlueprintAssignable)
	FOnLevelWon m_OnLevelWon;

	UPROPERTY(BlueprintAssignable)
	FOnLevelLost m_OnLevelLost;

private:

	TMap<AFOBActor*, ETowerType> m_Towers;

	float m_UltimateProgress = 0.f;

	uint32 m_Crowns = 1000;

	uint32 m_Scraps = 0;
};
