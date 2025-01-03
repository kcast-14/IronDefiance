// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "IDAIController.generated.h"

/**
 * 
 */
class AEnemy;
class ACharacterBase;
UCLASS()
class IRONDEFIANCE_API AIDAIController : public AAIController
{
	GENERATED_BODY()

public:
	AIDAIController(const FObjectInitializer& ObjectInitializer);


	FORCEINLINE bool IsPathPartial() { return bIsPathPartial; }
	FORCEINLINE void SetIsPathPartial(bool Partial) { bIsPathPartial = Partial; }

	template<typename T>
	T* GetOwningActor();

	template<typename T>
	void SetOwningActor(T* OwningActor);

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnMoveCompleted(FAIRequestID ID, const FPathFollowingResult& Result) override;

private:

	bool bIsPathPartial = false;

	bool bIsOwnedByEnemy = false;

	AActor* m_OwningActor = nullptr;

};


template<typename T>
inline T* AIDAIController::GetOwningActor()
{
	if (m_OwningActor == nullptr)
	{
		return nullptr;
	}

	return Cast<T>(m_OwningActor);
}

template<>
inline void AIDAIController::SetOwningActor<AEnemy>(AEnemy* OwningActor)
{
	m_OwningActor = Cast<AActor>(OwningActor);
	bIsOwnedByEnemy = true;

}

template<>
inline void AIDAIController::SetOwningActor<ACharacterBase>(ACharacterBase* OwningActor)
{
	m_OwningActor = Cast<AActor>(OwningActor);
}
