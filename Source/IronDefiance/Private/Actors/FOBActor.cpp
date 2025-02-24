// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FOBActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Controllers/IDPlayerController.h"
#include "Enemy/Enemy.h"
#include "GameModes/IDGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFOBActor::AFOBActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(m_CapsuleComp);
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	m_Mesh->SetupAttachment(GetRootComponent());

	m_AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Agro Sphere"));
	m_AgroSphere->SetupAttachment(m_Mesh);
	m_AgroSphere->InitSphereRadius(250.f);



}

void AFOBActor::OnAgroOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_OnDangerZoneEntered.Broadcast(OtherActor);

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		m_EnemiesInAgro.Add(Enemy);
	}
}

void AFOBActor::OnAgroOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//I doubt they would leave once they reach the base, but if the do or perhaps if they're destroyed we should let everyone know
	// Also I'm not sure whether enemies being destroyed would cause this delegate to be hit
	m_OnDangerZoneExited.Broadcast(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		m_EnemiesInAgro.Remove(Enemy);
	}
}

void AFOBActor::DecrementHealth(float Damage, FTimerHandle& Timer)
{

	if (m_Health - Damage <= 0.f)
	{
		m_Health = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		Die();
		return;
	}
	else
	{
		m_Health -= Damage;
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		return;
	}
}

void AFOBActor::Die()
{
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->RemoveTowerPointer(this);
}

// Called when the game starts or when spawned
void AFOBActor::BeginPlay()
{
	Super::BeginPlay();
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->AddTowerPointer(this);
	m_AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AFOBActor::OnAgroOverlapBegin);
	m_AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AFOBActor::OnAgroOverlapEnd);

	check(m_Type != ETowerType::DEFAULT_MAX);
}

// Called every frame
void AFOBActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (m_Type)
	{
	case ETowerType::TT_Crown:
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_ChargeTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(m_ChargeTimer, this, &AFOBActor::AddCrowns, m_Delay, false);
		}
		break;
	}
	case ETowerType::TT_Energy:
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_ChargeTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(m_ChargeTimer, this, &AFOBActor::IncrementUltimate, m_Delay, false);
		}
		break;
	}
	}

	/**
	* TODO: Have to find a better way to do this because if the array changes during the foreach loop it'll crash
	*/

	//if (!m_EnemiesInAgro.IsEmpty())
	//{
	//	for (auto& E : m_EnemiesInAgro)
	//	{
	//		if (!GetWorld()->GetTimerManager().IsTimerActive(E->GetTowerDamageTimer()))
	//		{
	//			FTimerDelegate Delegate;
	//			Delegate.BindUFunction(this, FName("DecrementHealth"), E->GetTowerDamage(), E->GetTowerDamageTimer());
	//			DecrementHealth(E->GetTowerDamage(), E->GetTowerDamageTimer());
	//			GetWorld()->GetTimerManager().SetTimer(E->GetTowerDamageTimer(), Delegate, m_Delay, false);
	//		}
	//		
	//	}
	//}
}

void AFOBActor::AddCrowns()
{
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->IncrementCrowns(m_CrownsToAdd);
	GetWorld()->GetTimerManager().ClearTimer(m_ChargeTimer);
}

void AFOBActor::IncrementUltimate()
{
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->IncrementEnergy(m_EnergyToAdd);
	GetWorld()->GetTimerManager().ClearTimer(m_ChargeTimer);

	if (Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentAmountEnergy() >= 1.f)
	{
		Cast<AIDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ToggleUltimateScreen();
	}
}

