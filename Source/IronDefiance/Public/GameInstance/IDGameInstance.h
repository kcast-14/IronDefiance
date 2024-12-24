// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IDGameInstance.generated.h"

/**
 * 
 */

class AWave;

UCLASS()
class IRONDEFIANCE_API UIDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
	AWave* GetWavePtr();

	void SetWavePtr(AWave* Pointer) { m_WavePtr = Pointer; }

protected:
	virtual void Init() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Wave", meta = (DisplayName = "Wave Instance"))
	AWave* m_WavePtr;
};
