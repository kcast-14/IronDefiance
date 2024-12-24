#pragma once

#include "CoreMinimal.h"
#include "IDStructs.generated.h"

USTRUCT(BlueprintType)
struct FTankStats
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats")
	float ArmorResistance = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxAPRounds = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxApcrRounds = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxHeatRounds = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Tank Stats|Ammo")
	float MaxExplosiveRounds = 5.f;
};
