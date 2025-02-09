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
        if (T.Value == ETowerType::TT_Crown)
        {
            CrownTowers.Add(T.Key);
        }
    }
    return CrownTowers;
}

TArray<AFOBActor*> AIDGameModeBase::GetEnergyTowers()
{
    TArray<AFOBActor*> EnergyTowers;
    for (auto& T : m_Towers)
    {
        if (T.Value == ETowerType::TT_Energy)
        {
            EnergyTowers.Add(T.Key);
        }
    }
    return EnergyTowers;
}

TArray<AFOBActor*> AIDGameModeBase::GetCommunicationsTowers()
{
    TArray<AFOBActor*> CommTowers;
    for (auto& T : m_Towers)
    {
        if (T.Value == ETowerType::TT_Comms)
        {
            CommTowers.Add(T.Key);
        }
    }
    return CommTowers;
}

void AIDGameModeBase::AddTowerPointer(AFOBActor* FOB)
{
    m_Towers.Emplace(FOB, FOB->GetTowerType());
}

void AIDGameModeBase::RemoveTowerPointer(AFOBActor* FOB)
{
    m_Towers.Remove(FOB);
    m_Towers.Shrink();
    // We're destroying here beucase if the player loses the Game AND they don't close the application afterwards this will probably still be living in memory which will cause
    // some major issues down the line.
    FOB->m_OnBaseDestroyed.Broadcast(this);
    FOB->Destroy();
    if (m_Towers.Num() == 0)
    {

        LoseGame();
        return;
    }

    return;
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

void AIDGameModeBase::WinGame()
{
    //GetWorld()->GetFirstPlayerController<AIDPlayerController>()->ToggleWinScreen();

    //Display visuals and play sounds that reinforce victory
}

void AIDGameModeBase::LoseGame()
{
    //GetWorld()->GetFirstPlayerController<AIDPlayerController>()->ToggleGameOverScreen();

    //Display visuals and play sounds that reinforce defeat
}
