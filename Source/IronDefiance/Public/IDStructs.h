#pragma once

#include "CoreMinimal.h"
#include "IDStructs.generated.h"

USTRUCT(BlueprintType)
struct FTankStats
{
	GENERATED_BODY();
	float MaxHealth = 10.f;
	float ArmorResistance = 1.f;
};
