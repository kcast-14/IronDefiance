// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/IDGameModeBase.h"
#include "Actors/FOBActor.h"

AIDGameModeBase::AIDGameModeBase()
{

}

TArray<AFOBActor*> AIDGameModeBase::GetCrownTowers()
{
    TArray<AFOBActor*> CrownTowers;
    for (auto& T : m_Towers)
    {
        if (T.Key == ETowerType::TT_Crown)
        {
            CrownTowers.Add(T.Value);
        }
    }
    return CrownTowers;
}

TArray<AFOBActor*> AIDGameModeBase::GetEnergyTowers()
{
    TArray<AFOBActor*> EnergyTowers;
    for (auto& T : m_Towers)
    {
        if (T.Key == ETowerType::TT_Energy)
        {
            EnergyTowers.Add(T.Value);
        }
    }
    return EnergyTowers;
}

TArray<AFOBActor*> AIDGameModeBase::GetCommunicationsTowers()
{
    TArray<AFOBActor*> CommTowers;
    for (auto& T : m_Towers)
    {
        if (T.Key == ETowerType::TT_Comms)
        {
            CommTowers.Add(T.Value);
        }
    }
    return CommTowers;
}

void AIDGameModeBase::AddTowerPointer(AFOBActor* FOB)
{
    m_Towers.Add(FOB->GetTowerType(), FOB);
}

void AIDGameModeBase::IncrementCrowns(uint32 Value)
{
    m_Crowns += Value;
}

void AIDGameModeBase::IncrementScrap(uint32 Value)
{
    m_Scraps += Value;
}

void AIDGameModeBase::IncrementEnergy(float Value)
{
    if (m_UltimateProgress + Value >= 1.f)
    {
        m_UltimateProgress = 1.f;
    }
    else
    {
        m_UltimateProgress += Value;
    }
}
