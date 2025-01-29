// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/IDSaveGame.h"


UIDSaveGame::UIDSaveGame()
{
	SlotName = "Autosave";
	SlotNumber = 0;
	UserIndex = 0;;
}

void UIDSaveGame::CreateSlot(int SaveSlotNumber)
{
	SlotName = CreateSaveSlotName(SaveSlotNumber);
	SlotNumber = SaveSlotNumber;
	UserIndex = 0;
}

void UIDSaveGame::CreateEmptySlot(int SaveSlotNumber)
{
	SlotName = "Empty Slot";
	SlotNumber = SaveSlotNumber;
	UserIndex = 0;
}

FString UIDSaveGame::CreateSaveSlotName(int SaveSlotNumber)
{
	if (SaveSlotNumber == 0)
	{
		return FString(("AutoSave"));
	}
	return FString(("Slot ")) + FString::FromInt(SaveSlotNumber);
}

FString UIDSaveGame::GetSlotName()
{
	return SlotName;
}