#include "Actors/Wave.h"
#include "GameInstance/IDGameInstance.h"
#include "Controllers/IDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Spawner/Spawner.h"
#include "Enemy/Enemy.h"

AWave::AWave()
{
	PrimaryActorTick.bCanEverTick = true;
	WaveNumber = 0;
	m_MaxNumberOfWaves = 1;
	m_InitialEnemyCount = 5;
	EnemyMaxCount = 0;
	EnemyRemaining = 0;
	TransitionPeriod = 5.0f;

}

void AWave::GetSpawners()
{
	//Get All spawners in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), Spawners);
}

void AWave::StartWave()
{
	EnemyMaxCount = CalculateMaxEnemyCount();
	this->BuildEnemyPool();
	GetWorld()->GetFirstPlayerController<AIDPlayerController>()->SetPoolSize(EnemyMaxCount);
	GetWorld()->GetFirstPlayerController<AIDPlayerController>()->MakeHealthBarWidgets();

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
				SelectedSpawner->AddEnemyToQueue(NewEnemy);
				
			}
		}
	}

	GetWorld()->GetFirstPlayerController<AIDPlayerController>()->RemoveWaveTransition();
	GetWorld()->GetFirstPlayerController<AIDPlayerController>()->ToggleHUDOverlay();
}

// Added: Delano Wilcox
void AWave::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance<UIDGameInstance>()->SetWavePtr(this); 
	GetSpawners();

	// We have to use these header guards because UE constructs objects in a different order in a shipping build 
	// So if we're packaging a game to ship, this class doesn't get called until AFTER the player controller has been constructed.
	// While in Editor, the actors in a level will be constructed first therefore we won't have a valid instance of Player Controller yet
#if UE_BUILD_SHIPPING
	GetWorld()->GetFirstPlayerController<AIDPlayerController>()->DisplayWaveTransition();
	EnterTransition();
#endif
}

void AWave::EnterTransition()
{
	if ((WaveNumber + 1) > m_MaxNumberOfWaves)
	{
		Win();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			TransitionTimer,
			this,
			&AWave::NewWave,
			TransitionPeriod,
			false
		);

	}

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
			GetWorld()->GetFirstPlayerController<AIDPlayerController>()->ToggleHUDOverlay();
			GetWorld()->GetFirstPlayerController<AIDPlayerController>()->DisplayWaveTransition(); // Added: Delano Wilcox
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

int AWave::CalculateMaxEnemyCount()
{
	return (int)(FMath::Sqrt((float)WaveNumber -1.f)*6.f +m_InitialEnemyCount); // Square root of the WaveNumber -1 times 6 plus 10
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

void AWave::Win()
{
	//GetWorld()->GetFirstPlayerController<AIDPlayerController>()->ToggleWinScreen();

	//Display visuals and play sounds that reinforce victory
}




