// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SettingsDataTable.generated.h"

/**
 * 
 */

class USoundClass;

UENUM()
enum class GraphicsQuality : int32
{
	GQ_Low UMETA(DisplayName = "Low"),
	GQ_Medium UMETA(DisplayName = "Medium"),
	GQ_High UMETA(DisplayName = "High"),
	GQ_Epic UMETA(DisplayName = "Epic"),
	GQ_Cinematic UMETA(DisplayName = "Cinematic"),
	DEFAULT_MAX UMETA(DisplayName = "Default")

};

USTRUCT(BlueprintType)
struct FIDAVSettings
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Width"))
	int32 X = 1920; // Width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Height"))
	int32 Y = 1080; // Height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Graphics Quality"))
	GraphicsQuality Quality = GraphicsQuality::GQ_Epic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Master Volume"))
	float MasterVolume = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Music Volume"))
	float MusicVolume = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (DisplayName = "Sound Effects Volume"))
	float SFXVolume = 1.f;


};
UCLASS()
class IRONDEFIANCE_API USettingsDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	USettingsDataTable(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable)
	FIntPoint GetResolutionXY() { return { m_AVSettings.X,m_AVSettings.Y }; }

	UFUNCTION(BlueprintCallable)
	int32 GetResolutionX() { return m_AVSettings.X; }
	UFUNCTION(BlueprintCallable)
	int32 GetResolutionY() { return m_AVSettings.Y; }
	UFUNCTION(BlueprintCallable)
	int32 GetGraphicsQuality() { return (int32)m_AVSettings.Quality; }
	UFUNCTION(BlueprintCallable)
	float GetMasterVolume() { return m_AVSettings.MasterVolume; }
	UFUNCTION(BlueprintCallable)
	float GetMusicVolume() { return m_AVSettings.MusicVolume; }
	UFUNCTION(BlueprintCallable)
	float GetSFXVolume() { return m_AVSettings.SFXVolume; }

	UFUNCTION(BlueprintCallable)
	void SetResolutionXY(FIntPoint Value) { m_AVSettings.X = Value.X; m_AVSettings.Y = Value.Y; }
	UFUNCTION(BlueprintCallable)
	void SetResolutionX(int32 Value) { m_AVSettings.X = Value; }
	UFUNCTION(BlueprintCallable)
	void SetResolutionY(int32 Value) { m_AVSettings.Y = Value; }
	UFUNCTION(BlueprintCallable)
	void SetGraphicsQuality(int32 Value) { m_AVSettings.Quality = (GraphicsQuality)Value; }
	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float Value) { m_AVSettings.MasterVolume = Value; }
	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float Value) { m_AVSettings.MusicVolume = Value; }
	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float Value) { m_AVSettings.SFXVolume = Value; }
	
	UFUNCTION(BlueprintCallable)
	void ApplySettings(UGameUserSettings* UserSettings);

	UFUNCTION(BlueprintCallable)
	void WriteSettings();

	UFUNCTION(BlueprintCallable)
	void ReadSettings();

	template<typename CharType = TCHAR>
	bool WriteIniFile();

	template<typename CharType = TCHAR>
	bool ReadIniFile();

	UFUNCTION(BlueprintCallable)
	FIDAVSettings& GetAVSettings() { return m_AVSettings; }


	friend FDataTableImporterJSON;

private:

	FString m_FilePath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "AV Settings"))
	FIDAVSettings m_AVSettings;

	
};
