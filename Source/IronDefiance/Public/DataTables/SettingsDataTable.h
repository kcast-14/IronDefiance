// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SettingsDataTable.generated.h"

/**
 * 
 */
UENUM()
enum class GraphicsQuality : uint8
{
	GQ_Low UMETA(DisplayName = "Low"),
	GQ_Medium UMETA(DisplayName = "Medium"),
	GQ_High UMETA(DisplayName = "High"),
	GQ_Ultra UMETA(DisplayName = "Ultra"),
	DEFAULT_MAX UMETA(DisplayName = "Default")

};

USTRUCT()
struct FIDAVSettings
{
	GENERATED_BODY()

	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Y;
	UPROPERTY()
	GraphicsQuality Quality = GraphicsQuality::GQ_Low;
	UPROPERTY()
	float MasterVolume = 0.f;
	UPROPERTY()
	float MusicVolume = 0.f;
	UPROPERTY()
	float SFXVolume = 0.f;


};
UCLASS()
class IRONDEFIANCE_API USettingsDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	USettingsDataTable();

	UFUNCTION(BlueprintCallable)
	void WriteSettings();

	UFUNCTION(BlueprintCallable)
	void ReadSettings();

	template<typename CharType = TCHAR>
	bool WriteIniFile();

	template<typename CharType = TCHAR>
	bool ReadIniFile();


	friend FDataTableImporterJSON;

private:

	FString m_FilePath;
	FIDAVSettings m_AVSettings;

	
};
