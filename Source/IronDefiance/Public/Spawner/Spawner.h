#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UCapsuleComponent;
class AEnemy;
class AWave;

UCLASS()
class IRONDEFIANCE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawner();

	virtual void Tick(float DeltaTime) override;
	void SpawnEnemy();
	void AddEnemyToQueue(const TSubclassOf<AEnemy>& Enemy);

	void SetWavePtr(AWave* WavePtr);
	
	UPROPERTY(BlueprintReadWrite)
	float SpawnStartDelay;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void PassPtrToEnemyClass(AEnemy& Enemy);

private:
	UCapsuleComponent* Capsule;
	FTimerHandle SpawnTimerHandle;
	TArray<TSubclassOf<AEnemy>> EnemyQueue; 

	AWave* m_WavePtr;
};
