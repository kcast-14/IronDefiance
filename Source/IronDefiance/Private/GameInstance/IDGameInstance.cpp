// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IDGameInstance.h"
void UIDGameInstance::Init()
{
	Super::Init();
}

AWave* UIDGameInstance::GetWavePtr()
{
	return m_WavePtr;
}


