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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
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
