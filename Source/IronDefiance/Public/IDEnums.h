#pragma once

#include "CoreMinimal.h"
#include "IDEnums.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AT_ArmorPiercing UMETA(DisplayName="Armor Piercing Round"),
	AT_Apcr UMETA(DisplayName="APCR Round"),
	AT_Heat UMETA(DisplayName="Heat Round"),
	AT_Explosive UMETA(DisplayName="Standard High Explosives Round"),
	DEFAULT_MAX UMETA(DisplayName= "Default")
};

UENUM(BlueprintType)
enum class ETankType : uint8
{
	TT_M4Sherman UMETA(DisplayName = "M4 Sherman"),
	TT_Tiger1 UMETA(DisplayName = "Tiger 1"),
	TT_M10Wolverine UMETA(DisplayName = "M10 Wolverine"),
	TT_EnemyVariant UMETA(DisplayName = "Enemy Variant"),
	DEFAULT_MAX UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	CM_FirstPerson UMETA(DisplayName = "First Person View"),
	CM_ThirdPerson UMETA(DisplayName = "Third Person View"),
	DEFAULT_MAX UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	UT_FireRate UMETA(DisplayName = "Fire Rate"),
	UT_Damage UMETA(DisplayName = "Damage"),
	UT_Health UMETA(DisplayName = "Health"),
	DEFAULT_MAX UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	MS_Idle UMETA(DisplayName = "Idle"),
	MS_MoveToTarget UMETA(DisplayName = "Move To Target"),
	MS_Attacking UMETA(DisplayName = "Attacking"),
	MS_Dead UMETA(DisplayName = "Dead"),
	DEFAULT_MAX UMETA(DisplayName = "Default")
};