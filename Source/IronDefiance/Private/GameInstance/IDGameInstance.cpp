// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IDGameInstance.h"
#include "Actors/Wave.h"
#include "Character/CharacterBase.h"
#include "Actors/FOBActor.h"
#include "GameModes/IDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/IDSaveGame.h"
#include <thread>


UIDGameInstance::UIDGameInstance()
{

}

void UIDGameInstance::MakeEmptyGameSave(int SlotToUse)
{
	UIDSaveGame* SaveGameInstance = Cast<UIDSaveGame>(UGameplayStatics::CreateSaveGameObject(UIDSaveGame::StaticClass()));

	m_CurrentSaveGame = SaveGameInstance;

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
	return;

}



void UIDGameInstance::Init()
{
	Super::Init();

	if (GetWorld())
	{
		BeginPlay();
	}
}

void UIDGameInstance::PostInitProperties()
{
	Super::PostInitProperties();


}

void UIDGameInstance::BeginPlay()
{
	m_SaveArray.Reserve(m_MaxNumberOfSaveFiles);
	for (int i = 0; i < m_MaxNumberOfSaveFiles; ++i)
	{
		m_SaveArray.Init({}, i);
	}

	if (UGameplayStatics::DoesSaveGameExist({ "AutoSave" }, 0))
	{
		m_CurrentSaveGame = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot({ "AutoSave" }, 0));
		m_SaveArray[0] = m_CurrentSaveGame;
	}
	// Delano: Yeah yeah yeah I know....
	for (int i = 1; i <= m_MaxNumberOfSaveFiles; ++i)
	{
		if (UGameplayStatics::DoesSaveGameExist(FString(("Slot ")) + FString::FromInt(i), 0))
		{
			UIDSaveGame* LoadGameInstance = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(("Slot ")) + FString::FromInt(i), 0));
			m_SaveArray[i] = LoadGameInstance;
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
	/**
	* This is my first time trying this so I'm not sure if there are bugs or how bug prone it is, but if there were to ever be issues with saving the game This would be the first place I'd look.
	* With that said, my thinking here is upon saving the game we'll grab whatever state the tanks were in and store that and anything that happens while the thread was running we wouldn't care about
	* because the player had starting saving the game from the point they clicked save, not afterwards.
	* One of the only issues i can think of off hand with this is what to do if the player is spamming the save button or something, but that can be guarded against by simply not allowing two saves to happen
	* to the save slot simultenously.
	*/

	TArray<AActor*> Tanks;
	TArray<AActor*> Towers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), Tanks);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFOBActor::StaticClass(), Towers);
	auto f = [&](TArray<AActor*> TankArray, TArray<AActor*> TowersArray, int Slot, bool IsAutosave, TArray<UIDSaveGame*> SaveArray)
	{
		if (!m_CurrentSaveGame)
		{
			UIDSaveGame* SaveGameInstance = Cast<UIDSaveGame>(UGameplayStatics::CreateSaveGameObject(UIDSaveGame::StaticClass()));

			SaveGameInstance->m_SaveInfo.Tanks.Empty();
			for (auto& T : TankArray)
			{
				ACharacterBase* Tank = Cast<ACharacterBase>(T);
				FTankInfo Info;
				Info.CurrentHealth = Tank->GetCurrentHealth();
				Info.CurrentAPRounds = Tank->GetCurrentAPRounds();
				Info.CurrentApcrRounds = Tank->GetCurrentApcrRounds();
				Info.CurrentExplosiveRounds = Tank->GetCurrentExplosiveRounds();
				Info.CurrentHeatRounds = Tank->GetCurrentHeatRounds();
				Info.Location = Tank->GetActorLocation();
				Info.Rotation = Tank->GetActorRotation();
				Info.Stats = Tank->GetStats();
				SaveGameInstance->m_SaveInfo.Tanks.Add(Info);
			}

			for (auto& T : TowersArray)
			{
				AFOBActor* Tower = Cast<AFOBActor>(T);
				FTowerInfo Info;
				Info.Location = Tower->GetActorLocation();
				Info.Rotation = Tower->GetActorRotation();
				Info.Type = Tower->GetTowerType();
				Info.Health = Tower->GetHealth();
				Info.TimerDelay = Tower->GetDelay();
				Info.EnergyToAdd = Tower->GetEnergyRate();
				Info.CrownsToAdd = Tower->GetCrownsToAdd();
				SaveGameInstance->m_SaveInfo.Towers.Add(Info);
			}

			if (m_WavePtr != nullptr)
			{
				SaveGameInstance->m_SaveInfo.CurrentWaveNumber = m_WavePtr->GetWaveNumber();
			}
			else
			{
				SaveGameInstance->m_SaveInfo.CurrentWaveNumber = 0;
			}
			SaveGameInstance->m_SaveInfo.Crowns = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentNumCrowns();
			SaveGameInstance->m_SaveInfo.Scraps = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentNumScraps();
			SaveGameInstance->m_SaveInfo.Energy = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentAmountEnergy();
			FString InDate = FDateTime::Now().GetDate().ToString();
			FString InTime = FDateTime::Now().GetTimeOfDay().ToString();
			FString Date;
			FString Time;
			ParseDate(InDate, Date);
			ParseTime(InTime, Time);
			SaveGameInstance->m_SaveInfo.Date = Date;
			SaveGameInstance->m_SaveInfo.Time = Time;

			FString MapName = GetWorld()->GetMapName();
			MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			SaveGameInstance->m_SaveInfo.Mapname = MapName;

			m_CurrentSaveGame = SaveGameInstance;
		}
		else
		{

			m_CurrentSaveGame->m_SaveInfo.Tanks.Empty();
			for (auto& T : TankArray)
			{
				ACharacterBase* Tank = Cast<ACharacterBase>(T);
				FTankInfo Info;
				Info.CurrentHealth = Tank->GetCurrentHealth();
				Info.CurrentAPRounds = Tank->GetCurrentAPRounds();
				Info.CurrentApcrRounds = Tank->GetCurrentApcrRounds();
				Info.CurrentExplosiveRounds = Tank->GetCurrentExplosiveRounds();
				Info.CurrentHeatRounds = Tank->GetCurrentHeatRounds();
				Info.Location = Tank->GetActorLocation();
				Info.Rotation = Tank->GetActorRotation();
				Info.Stats = Tank->GetStats();
				m_CurrentSaveGame->m_SaveInfo.Tanks.Add(Info);
			}

			for (auto& T : TowersArray)
			{
				AFOBActor* Tower = Cast<AFOBActor>(T);
				FTowerInfo Info;
				Info.Location = Tower->GetActorLocation();
				Info.Rotation = Tower->GetActorRotation();
				Info.Type = Tower->GetTowerType();
				Info.Health = Tower->GetHealth();
				Info.TimerDelay = Tower->GetDelay();
				Info.EnergyToAdd = Tower->GetEnergyRate();
				Info.CrownsToAdd = Tower->GetCrownsToAdd();
				m_CurrentSaveGame->m_SaveInfo.Towers.Add(Info);
			}

			m_CurrentSaveGame->m_SaveInfo.CurrentWaveNumber = m_WavePtr->GetWaveNumber();
			m_CurrentSaveGame->m_SaveInfo.Crowns = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentNumCrowns();
			m_CurrentSaveGame->m_SaveInfo.Scraps = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentNumScraps();
			m_CurrentSaveGame->m_SaveInfo.Energy = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentAmountEnergy();
			FString Date;
			FString Time;
			ParseDate(FDateTime::Now().GetDate().ToString(), Date);
			ParseTime(FDateTime::Now().GetTimeOfDay().ToString(), Time);
			m_CurrentSaveGame->m_SaveInfo.Date = Date;
			m_CurrentSaveGame->m_SaveInfo.Time = Time;
			FString MapName = GetWorld()->GetMapName();
			MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			m_CurrentSaveGame->m_SaveInfo.Mapname = MapName;
		}

		if (IsAutosave)
		{
			SaveArray[0] = m_CurrentSaveGame;
			m_CurrentSaveGame->CreateSlot(0);
			UGameplayStatics::SaveGameToSlot(m_CurrentSaveGame, m_CurrentSaveGame->SlotName, m_CurrentSaveGame->UserIndex);
			return;
		}

		if (Slot != 0)
		{
			SaveArray[Slot] = m_CurrentSaveGame;
			m_CurrentSaveGame->CreateSlot(Slot);
			UGameplayStatics::SaveGameToSlot(m_CurrentSaveGame, m_CurrentSaveGame->SlotName, m_CurrentSaveGame->UserIndex);
			return;
		}
		else
		{
			ensureMsgf(false, TEXT("Slot 0 is reserved for AutoSaves"));
		}
		return;
	};

	std::thread th(f, Tanks, Towers, SlotToUse, bIsAutosave, m_SaveArray);

	th.detach();
}

void UIDGameInstance::LoadGame(FString SaveSlotName)
{
	UIDSaveGame* LoadGameInstance = Cast<UIDSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	m_CurrentSaveGame = LoadGameInstance;

	if (LoadGameInstance != nullptr)
	{
		//Loading the level first so that way we can be sure we have the correct Pointer to the Wave Class
		
		if (LoadGameInstance->m_SaveInfo.Mapname != TEXT(""))
		{
			FName MapName(*LoadGameInstance->m_SaveInfo.Mapname);
			SwitchLevel(MapName);
		}
		
		FActorSpawnParameters SpawnParams;
		ACharacterBase* Tank;
		AFOBActor* Tower;

		for (auto& T : LoadGameInstance->m_SaveInfo.Towers)
		{
			Tower = GetWorld()->SpawnActor<AFOBActor>(
				AFOBActor::StaticClass(),
				T.Location, T.Rotation, SpawnParams
			);
	
			Tower->SetTowerType(T.Type);
			Tower->SetHealth(T.Health);
			Tower->SetDelay(T.TimerDelay);
			Tower->SetEnergyRate(T.EnergyToAdd);
			Tower->SetCrownsToAdd(T.CrownsToAdd);
		}
	
		for (auto& T : LoadGameInstance->m_SaveInfo.Tanks)
		{
			Tank = GetWorld()->SpawnActor<ACharacterBase>(
				ACharacterBase::StaticClass(),
				T.Location, T.Rotation, SpawnParams
			);
	
			Tank->SetCurrentHealth(T.CurrentHealth);
			Tank->SetCurrentAPRounds(T.CurrentAPRounds);
			Tank->SetCurrentApcrRounds(T.CurrentApcrRounds);
			Tank->SetCurrentExplosiveRounds(T.CurrentExplosiveRounds);
			Tank->SetCurrentHeatRounds(T.CurrentHeatRounds);
			Tank->SetStats(T.Stats);
		}
	
	
	
		m_CurrentWaveNumber = LoadGameInstance->m_SaveInfo.CurrentWaveNumber;
		Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetCurrentNumCrowns(LoadGameInstance->m_SaveInfo.Crowns);
		Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetCurrentNumScraps(LoadGameInstance->m_SaveInfo.Scraps);
		Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetCurrentAmountEnergy(LoadGameInstance->m_SaveInfo.Energy);

		bLoadedSave = true;
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


