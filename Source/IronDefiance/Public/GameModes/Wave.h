#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Wave.generated.h"

class AEnemy;

UCLASS()
class IRONDEFIANCE_API AWave : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWave();
	UPROPERTY(BlueprintReadOnly)
	int WaveNumber;
	UPROPERTY(BlueprintReadOnly)
	int EnemyRemaining;
	UPROPERTY(BlueprintReadOnly)
	TMap<TSubclassOf<AEnemy>, int> EnemyPool;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TransitionTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float TransitionPeriod;

	UFUNCTION(BlueprintCallable)
	void OnEnemyDefeated();
	UFUNCTION(BlueprintCallable)
	void NewWave();

protected:
	void GetSpawners();
	void StartWave();
	void EndGame();
	void EnterTransition();
	void BuildEnemyPool();
	int GetPoolSize() const;
	TSubclassOf<AEnemy> GetEnemyFromPool() const;
	
	
	UPROPERTY(BlueprintReadOnly)
	int EnemyMaxCount;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Spawners;
};
