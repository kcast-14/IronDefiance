#pragma once

#include "CoreMinimal.h"
#include "IDEnums.h"
#include "IDStructs.generated.h"

USTRUCT(BlueprintType)
struct FTankStats
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float ArmorResistance = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float Range = 10.f;
	//Measured in Rounds Per Second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats", meta=(ToolTip = "This is measured in rounds per second"))
	float FireRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float DamageAddition = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxAPRounds = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxApcrRounds = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxHeatRounds = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxExplosiveRounds = 5.f;



	bool UpgradeStat(float Value, EUpgradeType Type)
	{
		switch (Type)
		{
		case EUpgradeType::UT_FireRate:
		{
			FireRate += Value;
			return true;
			break;
		}
		case EUpgradeType::UT_Damage:
		{
			DamageAddition += Value;
			return true;
			break;
		}
		case EUpgradeType::UT_Health:
		{
			MaxHealth += Value;
			return true;
			break;
		}
		default:
		{
			return false;
		}
		}
	}

};

USTRUCT(BlueprintType)
struct FTankInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentHealth = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentAPRounds = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentApcrRounds = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentHeatRounds = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentExplosiveRounds = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FTankStats Stats;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator  Rotation;
};

USTRUCT(BlueprintType)
struct FTowerInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator  Rotation;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	ETowerType Type;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float TimerDelay;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	uint32 CrownsToAdd;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float EnergyToAdd;

};


USTRUCT(BlueprintType)
struct FSaveInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	TArray<FTankInfo> Tanks;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	TArray<FTowerInfo> Towers;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int CurrentWaveNumber;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	uint32 Crowns;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	uint32 Scraps;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Energy;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString Mapname;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameData")
	FString Date;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameData")
	FString Time;
};
