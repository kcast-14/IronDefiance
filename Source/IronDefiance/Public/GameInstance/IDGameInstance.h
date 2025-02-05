// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IDGameInstance.generated.h"

/**
 * 
 */

class AWave;
class UIDSaveGame;

UCLASS()
class IRONDEFIANCE_API UIDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UIDGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void MakeEmptyGameSave(int SlotToUse);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveGame(int SlotToUse, bool IsAutoSaving);
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadGame(FString SaveSlotName);
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadGameNoSwitch(bool bSetPosition, FString SaveSlotName);

	void SwitchLevel(FName MapName);

	UFUNCTION(BlueprintCallable)
	AWave* GetWavePtr();

	void SetWavePtr(AWave* Pointer) { m_WavePtr = Pointer; }

protected:
	virtual void Init() override;
	virtual void PostInitProperties() override;
	void BeginPlay();

	bool ParseDate(const FString& DateTimeString, FString& OutDate);
	bool ParseTime(const FString& DateTimeString, FString& OutTime);

private:

	UPROPERTY(VisibleAnywhere, Category = "Wave", meta = (DisplayName = "Wave Instance"))
	AWave* m_WavePtr;

	UPROPERTY(VisibleAnywhere, Category = "Save", meta = (DisplayName = "Save Array"))
	TArray<UIDSaveGame*> m_SaveArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Current Save Game"))
	UIDSaveGame* m_CurrentSaveGame = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Max Number Of Saves Slots"))
	int m_MaxNumberOfSaveFiles = 11;

	bool bIsAutosave = false;

	uint32 m_Crowns = 0;
	uint32 m_Scraps = 0;


};
