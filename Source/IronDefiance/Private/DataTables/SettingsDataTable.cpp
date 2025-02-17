// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTables/SettingsDataTable.h"
#include "JsonObjectConverter.h"

USettingsDataTable::USettingsDataTable()
{
	FString Path = FPaths::ProjectSavedDir();
	Path.Append("AVSettings.ini");
	m_FilePath = Path;
	if (!FPaths::FileExists(m_FilePath))
	{
		FFileHelper::SaveStringToFile(FString(),*m_FilePath);
	}
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
