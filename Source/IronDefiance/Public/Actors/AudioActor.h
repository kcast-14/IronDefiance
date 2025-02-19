// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"
#include "AudioActor.generated.h"

UCLASS()
class IRONDEFIANCE_API AAudioActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
	UBillboardComponent* m_BillboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UAudioComponent* m_AudioComponent;

	UFUNCTION()
	void OnGameWon();

	UFUNCTION()
	void OnGameLost();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Win BGM"))
	USoundBase* m_WinSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "Lose BGM"))
	USoundBase* m_LoseSound;

};
