// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IDStructs.h"
#include "IDSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class IRONDEFIANCE_API UIDSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic)
	FString SlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic)
	int SlotNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic, meta=(DisplayName="Save Info"))
	FSaveInfo m_SaveInfo;

public:

	//Functions
	UIDSaveGame();

	void CreateSlot(int SaveSlotNumber);

	void CreateEmptySlot(int SaveSlotNumber);

	FString CreateSaveSlotName(int SaveSlotNumber);

	FString GetSlotName();
};
