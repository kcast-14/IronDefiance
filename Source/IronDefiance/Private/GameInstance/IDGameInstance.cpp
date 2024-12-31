// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IDGameInstance.h"
#include "SaveGame/IDSaveGame.h"
#include "Kismet/GameplayStatics.h"


UIDGameInstance::UIDGameInstance()
{

}



void UIDGameInstance::Init()
{
	Super::Init();
}

void UIDGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetWorld())
	{
		BeginPlay();
	}
}

void UIDGameInstance::BeginPlay()
{
	m_SaveArray.Reserve(m_MaxNumberOfSaveFiles);

	if (UGameplayStatics::DoesSaveGameExist({ "AutoSave" }, 0))
	{
		m_CurrentSaveGame = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot({ "AutoSave" }, 0));
		m_SaveArray[0] = m_CurrentSaveGame;
	}
	// Delano: Yeah yeah yeah I know....
	for (int i = 1; i <= m_MaxNumberOfSaveFiles; ++i)
	{
		if (UGameplayStatics::DoesSaveGameExist(FString(("Slot %d"), i), 0))
		{
			UIDSaveGame* LoadGameInstance = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(("Slot %d"), i), 0));
			m_SaveArray.Insert(LoadGameInstance, i);
		}
	}
}


AWave* UIDGameInstance::GetWavePtr()
{
	return m_WavePtr;
}

void UIDGameInstance::SaveGame(int SlotToUse, bool IsAutoSaving)
{
	bIsAutosave = IsAutoSaving;

	if (!m_CurrentSaveGame)
	{
		UIDSaveGame* SaveGameInstance = Cast<UIDSaveGame>(UGameplayStatics::CreateSaveGameObject(UIDSaveGame::StaticClass()));
		FString InDate = FDateTime::Now().GetDate().ToString();
		FString InTime = FDateTime::Now().GetTimeOfDay().ToString();
		FString Date;
		FString Time;
		ParseDate(InDate, Date);
		ParseTime(InTime, Time);
		FString MapName = GetWorld()->GetMapName();
		MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		m_CurrentSaveGame = SaveGameInstance;
	}
	else
	{
		FString Date;
		FString Time;
		ParseDate(FDateTime::Now().GetDate().ToString(), Date);
		ParseTime(FDateTime::Now().GetTimeOfDay().ToString(), Time);
		FString MapName = GetWorld()->GetMapName();
		MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	}

	if (bIsAutosave)
	{
		m_SaveArray[0] = m_CurrentSaveGame;
		m_CurrentSaveGame->CreateSlot(0);
		UGameplayStatics::SaveGameToSlot(m_CurrentSaveGame, m_CurrentSaveGame->SlotName, m_CurrentSaveGame->UserIndex);
		return;
	}

	if (SlotToUse != 0)
	{
		m_SaveArray[SlotToUse] = m_CurrentSaveGame;
		m_CurrentSaveGame->CreateSlot(SlotToUse);
		UGameplayStatics::SaveGameToSlot(m_CurrentSaveGame, m_CurrentSaveGame->SlotName, m_CurrentSaveGame->UserIndex);
		return;
	}
	else
	{
		ensureMsgf(false, TEXT("Slot 0 is reserved for AutoSaves"));
	}



}

void UIDGameInstance::LoadGame(FString SaveSlotName)
{
	UIDSaveGame* LoadGameInstance = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	m_CurrentSaveGame = LoadGameInstance;

	if (LoadGameInstance)
	{
		//if (LoadGameInstance->SaveStats.MapName != TEXT(""))
		//{
		//	FName MapName(*LoadGameInstance->SaveStats.MapName);
		//	SwitchLevel(MapName);
		//}

	}
}

void UIDGameInstance::LoadGameNoSwitch(bool bSetPosition, FString SaveSlotName)
{
	UIDSaveGame* LoadGameInstance = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	m_CurrentSaveGame = LoadGameInstance;

	if (LoadGameInstance)
	{

	}
}

void UIDGameInstance::SwitchLevel(FName MapName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();

		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != MapName)
		{
			UGameplayStatics::OpenLevel(World, MapName);
		}
	}
}

//TODO: I'll probably rewrite these later to see if I can do it in a not so verbose way
bool UIDGameInstance::ParseDate(const FString& DateTimeString, FString& OutDate)
{
	TArray<FString> StringArray;
	FString FixedString = DateTimeString.Replace(TEXT("-"), TEXT(" "));
	FixedString.ReplaceInline(TEXT(":"), TEXT(" "), ESearchCase::CaseSensitive);
	FixedString.ReplaceInline(TEXT("."), TEXT("/"), ESearchCase::CaseSensitive);

	FixedString.ParseIntoArray(StringArray, TEXT(" "), true);

	if ((StringArray.Num() < 2 || StringArray.Num() == 0))
	{
		return false;
	}

	for (int32 i = 0; i < (StringArray.Num() - 1); ++i)
	{
		OutDate.Append(StringArray[i]);
	}

	return true;
}

bool UIDGameInstance::ParseTime(const FString& DateTimeString, FString& OutTime)
{
	TArray<FString> StringArray;
	FString FixedString = DateTimeString.Replace(TEXT("+"), TEXT(" "));
	FixedString.ReplaceInline(TEXT("."), TEXT(" "), ESearchCase::CaseSensitive);

	FixedString.ParseIntoArray(StringArray, TEXT(" "), true);

	if ((StringArray.Num() < 2 || StringArray.Num() == 0))
	{
		return false;
	}

	for (int32 i = 0; i < (StringArray.Num() - 1); ++i)
	{
		OutTime.Append(StringArray[i]);
	}
	return true;
}


