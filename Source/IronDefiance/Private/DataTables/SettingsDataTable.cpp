// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTables/SettingsDataTable.h"
#include "JsonObjectConverter.h"
#include "GameInstance/IDGameInstance.h"
#include "GameModes/IDGameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

USettingsDataTable::USettingsDataTable(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FString Path = FPaths::ProjectSavedDir();
	Path.Append("AVSettings.ini");
	m_FilePath = Path;
	if (!FPaths::FileExists(m_FilePath))
	{
		WriteSettings();
		
	}
	else
	{
		ReadSettings();
	}
}

void USettingsDataTable::ApplySettings(UGameUserSettings* UserSettings)
{

	AIDGameModeBase* GameMode = Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(this));
	UIDGameInstance* GameInstance = Cast<UIDGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (!GameMode)
	{
		ensureMsgf(GameMode, TEXT("GameMode was not Valid!"));
		return;
	}

	if (!GameInstance)
	{
		ensureMsgf(GameInstance, TEXT("GameInstance was not Valid!"));
		return;
	}
	//Video
	UserSettings->SetScreenResolution({ m_AVSettings.X,m_AVSettings.Y });
	UserSettings->SetOverallScalabilityLevel((int32)m_AVSettings.Quality);
	UserSettings->ApplySettings(false);

	//Audio

	
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameInstance->GetSoundMix(), GameMode->GetMaster(), m_AVSettings.MasterVolume);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameInstance->GetSoundMix(), GameMode->GetMusic(), m_AVSettings.MusicVolume);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameInstance->GetSoundMix(), GameMode->GetSFX(), m_AVSettings.SFXVolume);

	WriteSettings();

}

void USettingsDataTable::WriteSettings()
{
	WriteIniFile();
}

void USettingsDataTable::ReadSettings()
{
	ReadIniFile();
}

template<typename CharType>
bool USettingsDataTable::WriteIniFile()
{
	FString AV;
	FJsonObjectConverter::UStructToJsonObjectString(m_AVSettings, AV);

	FString Settings;

	Settings.Append(AV);
	if (FFileHelper::SaveStringToFile(Settings, *m_FilePath))
	{
		return true;
	}
	else
	{
		return false;
	}
}


template<typename CharType>
bool USettingsDataTable::ReadIniFile()
{
	FString AV;

	if (FFileHelper::LoadFileToString(AV, *m_FilePath))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct<FIDAVSettings>(AV, &m_AVSettings);
		return true;
	}
	else
	{
		return false;
	}
}
