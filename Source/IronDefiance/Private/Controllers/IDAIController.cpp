// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/IDAIController.h"
#include "Navigation/PathFollowingComponent.h"

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
