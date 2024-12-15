#include "GameModes/Wave.h"
#include "Kismet/GameplayStatics.h"
#include "Spawner/Spawner.h"
#include "Enemy/Enemy.h"

AWave::AWave()
{
	PrimaryActorTick.bCanEverTick = true;
	WaveNumber = 0;
	EnemyMaxCount = 5;
	EnemyRemaining = 0;
	TransitionPeriod = 5.0f;
	this->GetSpawners();
}

void AWave::GetSpawners()
{
	//Get All spawners in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), Spawners);
}

void AWave::StartWave()
{
	this->BuildEnemyPool();

	//The amount of enemies in the field during a round will always be based on EnemyMaxCount
	//EnemyMaxCount can be changed to maximize the difficult of the rounds
	int spawnNum = FMath::Min(GetPoolSize(), EnemyMaxCount);

	for (int i = 0; i < spawnNum; i++)
	{
		//This will spawn the enemies in a random spawner
		int32 randomEnemyIndex = FMath::RandRange(0, Spawners.Num() - 1);
		ASpawner* SelectedSpawner = Cast<ASpawner>(Spawners[randomEnemyIndex]);

		if (SelectedSpawner != nullptr)
		{
			TSubclassOf<AEnemy> NewEnemy = this->GetEnemyFromPool();
			if (NewEnemy != nullptr)
			{
				// Delano: In order to call OnEnemyDefeated in the Enemy class we need a Ref/Ptr to the wave class,
				// and the spawner will be used to pass that Ptr onto the Enemy class since it already acts as a manager/factory
				SelectedSpawner->SetWavePtr(this);
				SelectedSpawner->AddEnemyToQueue(NewEnemy);
				
			}
		}
	}
}

void AWave::EndGame()
{
	//Check if win or lose
}

void AWave::EnterTransition()
{
	GetWorld()->GetTimerManager().SetTimer(
		TransitionTimer,       
		this,               
		&AWave::NewWave,  
		TransitionPeriod,                  
		false            
	);
}

void AWave::BuildEnemyPool()
{
	EnemyRemaining = this->GetPoolSize();
}

int AWave::GetPoolSize() const
{
	//Array of the amount of different types of enemies
	TArray<int> EnemyCounts;
	EnemyPool.GenerateValueArray(EnemyCounts);

	int sum = 0;

	for (auto EnemyCount : EnemyCounts)
	{
		sum += EnemyCount;
	}

	return sum;
}

void AWave::OnEnemyDefeated()
{
	EnemyRemaining--;

	//can spawn more enemies
	if (EnemyRemaining >= EnemyMaxCount)
	{
		int32 randomEnemyIndex = FMath::RandRange(0, Spawners.Num() - 1);
		ASpawner* SelectedSpawner = Cast<ASpawner>(Spawners[randomEnemyIndex]);

		if (SelectedSpawner != nullptr)
		{
			TSubclassOf<AEnemy> NewEnemy = this->GetEnemyFromPool();
			if (NewEnemy != nullptr)
			{
				SelectedSpawner->AddEnemyToQueue(NewEnemy);
			}
		}
	}
	else
	{
		//Go to the next round
		if (EnemyRemaining <= 0)
		{
			this->EnterTransition();
		}
	}
}


TSubclassOf<AEnemy> AWave::GetEnemyFromPool() const
{
	//Get different enemies
	if (EnemyPool.Num() > 0) 
	{
		TArray<TSubclassOf<AEnemy>> Keys;
		EnemyPool.GetKeys(Keys);
		TSubclassOf<AEnemy> EnemyKey = Keys[0];
		return EnemyKey;
	}

	return nullptr;
}


void AWave::NewWave()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TransitionTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(TransitionTimer);
	}
	
	WaveNumber += 1;
	this->StartWave();
}




