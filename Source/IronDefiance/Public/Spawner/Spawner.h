#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UCapsuleComponent;
class AEnemy;

UCLASS()
class IRONDEFIANCE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawner();

	virtual void Tick(float DeltaTime) override;
	void SpawnEnemy();
	void AddEnemyToQueue(const TSubclassOf<AEnemy>& Enemy);
	
	UPROPERTY(BlueprintReadWrite)
	float SpawnStartDelay;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UCapsuleComponent* Capsule;
	FTimerHandle SpawnTimerHandle;
	TArray<TSubclassOf<AEnemy>> EnemyQueue; 
};
