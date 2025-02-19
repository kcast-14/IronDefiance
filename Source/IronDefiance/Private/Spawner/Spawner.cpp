#include "Spawner/Spawner.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Wave.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"


ASpawner::ASpawner()
{
	//Creating a capsule component
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleSize(44.f, 44.f, false);
	SetRootComponent(Capsule);

	SpawnStartDelay = 10.0f;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	//Delay 
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,       
		this,               
		&ASpawner::SpawnEnemy,  
		SpawnStartDelay,                  
		true            
	);
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}


void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnEnemy();
}

void ASpawner::SpawnEnemy()
{
	//Every Spawner has a Queue of enemies to spawn
	if (GetWorld() != nullptr && EnemyQueue.IsValidIndex(0))
	{
		FTransform SpawnTransform = GetActorTransform();
		
		TArray<AActor*> OverlappingEnemies;
		GetOverlappingActors(OverlappingEnemies, AEnemy::StaticClass());

		FVector Offset = FVector(FMath::RandRange(-200.0f, 200.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f);
		SpawnTransform.SetLocation(SpawnTransform.GetLocation() + Offset);
		
		//Spawns an Enemy
		AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(
			EnemyQueue[0],
			SpawnTransform
		);

		//Remove Enemy from Queue
		if (SpawnedEnemy != nullptr)
		{
			EnemyQueue.RemoveAt(0);
		}
	}
}

void ASpawner::AddEnemyToQueue(const TSubclassOf<AEnemy>& Enemy)
{
	if (Enemy != nullptr) 
	{
		EnemyQueue.Add(Enemy);
	}

	TArray<AActor*> OverlappingActors;
	Capsule->GetOverlappingActors(OverlappingActors);

	if(!OverlappingActors.IsValidIndex(0))
	{
		SpawnEnemy();
	}
}


