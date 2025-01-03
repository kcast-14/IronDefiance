// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FOBActor.h"
#include "Components/SphereComponent.h"
#include "GameModes/IDGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFOBActor::AFOBActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	m_AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Agro Sphere"));
	m_AgroSphere->SetupAttachment(m_Mesh);
	m_AgroSphere->InitSphereRadius(250.f);



}

void AFOBActor::OnAgroOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_OnDangerZoneEntered.Broadcast(OtherActor);
}

void AFOBActor::OnAgroOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//I doubt they would leave once they reach the base, but if the do or perhaps if they're destroyed we should let everyone know
	// Also I'm not sure whether enemies being destroyed would cause this delegate to be hit
	m_OnDangerZoneExited.Broadcast(OtherActor);
}

// Called when the game starts or when spawned
void AFOBActor::BeginPlay()
{
	Super::BeginPlay();
	Cast<AIDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetFOBPointer(this);
	m_AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AFOBActor::OnAgroOverlapBegin);
	m_AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AFOBActor::OnAgroOverlapEnd);
}

// Called every frame
void AFOBActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

