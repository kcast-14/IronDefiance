// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/IDAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Character/CharacterBase.h"
#include "Enemy/Enemy.h"

AIDAIController::AIDAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPathFollowingComponent>(TEXT("PathFollowingComponent")))
{

}

void AIDAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AIDAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AIDAIController::OnMoveCompleted(FAIRequestID ID, const FPathFollowingResult& Result)
{
	if (bIsPathPartial && bIsOwnedByEnemy)
	{
		if (GetOwningActor<AEnemy>()->IsPlayerBlockingPath())
		{
			if (GetOwningActor<AEnemy>()->CanNavigateAround())
			{
				GetOwningActor<AEnemy>()->MoveToTarget();
			}
			else
			{
				GetOwningActor<AEnemy>()->PrepareToAttack();
			}
		}
		else
		{
			GetOwningActor<AEnemy>()->SetMovementStatus(EMovementStatus::MS_Idle);
			//We're stuck on top of something
		}
	}
	else
	{
		//Do all of the same stuff but with the ACharacterBase specialization
	}
}



