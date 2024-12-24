#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wave.generated.h"

class AEnemy;

UCLASS()
class IRONDEFIANCE_API AWave : public AActor
{
	GENERATED_BODY()

public:
	AWave();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Information")
	int WaveNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Information")
	int EnemyRemaining;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Information")
	TMap<TSubclassOf<AEnemy>, int> EnemyPool;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TransitionTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Information");
	float TransitionPeriod;

	UFUNCTION(BlueprintCallable)
	void OnEnemyDefeated();
	UFUNCTION(BlueprintCallable)
	void NewWave();

protected:
	void GetSpawners();
	void StartWave();
	virtual void BeginPlay() override;
	void EndGame();
	void EnterTransition();
	void BuildEnemyPool();
	int GetPoolSize() const;
	TSubclassOf<AEnemy> GetEnemyFromPool() const;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Information")
	int EnemyMaxCount;

private:
	UPROPERTY(VisibleAnywhere, Category = "Wave Information")
	TArray<AActor*> Spawners;

	friend class AIDPlayerController;
};
