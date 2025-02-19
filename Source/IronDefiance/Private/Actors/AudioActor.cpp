// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AudioActor.h"
#include "GameModes/IDGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAudioActor::AAudioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard Component"));
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	m_AudioComponent->SetupAttachment(m_BillboardComponent);
	SetRootComponent(m_BillboardComponent);

}

void AAudioActor::OnGameWon()
{
	m_AudioComponent->SetActiveFlag(false);
	m_AudioComponent->Stop();

	if (m_WinSound)
	{
		UGameplayStatics::PlaySound2D(this, m_WinSound, 1.f, 1.f, 0.f, nullptr, nullptr, false);
	}

}

void AAudioActor::OnGameLost()
{
	m_AudioComponent->SetActiveFlag(false);
	m_AudioComponent->Stop();
	if (m_LoseSound)
	{
		UGameplayStatics::PlaySound2D(this, m_LoseSound, 1.f, 1.f, 0.f, nullptr, nullptr, false);
	}
}

// Called when the game starts or when spawned
void AAudioActor::BeginPlay()
{
	Super::BeginPlay();

	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(this))->m_OnLevelWon.AddDynamic(this, &AAudioActor::OnGameWon);
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(this))->m_OnLevelLost.AddDynamic(this, &AAudioActor::OnGameLost);
	
}

// Called every frame
void AAudioActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

