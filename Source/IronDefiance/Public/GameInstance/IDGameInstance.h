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

	UFUNCTION(BlueprintCallable)
	int GetCurrentSaveSlotNumber() { return m_CurrentSlotInUse; }

	//This will potentially be moved to GameMode since there should be a new Game Mode made everytime we load a new Level. However it will be here for now because I can ensure that the value 
	//it holds here will be the correct one for saving and loading.
	int GetCurrentWaveNumber() { return m_CurrentWaveNumber; }

	void SetCurrentWaveNumber(int Value) { m_CurrentWaveNumber = Value; }

	bool IsLoadedSave() { return bLoadedSave; }

	// Will set the bLoadedSave variable to the opposite of what is passed. So if you pass true, then it'll reset the variable to false, if you pass false (for whatever reason) it'll set it to true.
	void IsFinishedLevelSetup(bool Value) { bLoadedSave = !Value; }

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

	bool bLoadedSave = false;

	int m_CurrentWaveNumber = 0;

	int m_CurrentSlotInUse = 0;


};
