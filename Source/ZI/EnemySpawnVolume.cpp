// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "ZIGameMode.h"

// Sets default values
AEnemySpawnVolume::AEnemySpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	//spawn delay range
	EnemySpawnDelayLow = 2.0f;
	EnemySpawnDelayHigh = 4.0f;

	
}

// Called when the game starts or when spawned
void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	//SpawnPickup();
}

// Called every frame
void AEnemySpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AEnemySpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AEnemySpawnVolume::SetSpawnActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		SpawnDelay = FMath::FRandRange(EnemySpawnDelayLow, EnemySpawnDelayHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnVolume::SpawnPickup, SpawnDelay, false);
		//SpawnPickup();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void AEnemySpawnVolume::SpawnPickup()
{

	if (WhatToSpawn != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
				//Set the spawn parameters
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				//Get a random location to spawn at
				FVector SpawnLocation = GetRandomPointInVolume();
				FRotator SpawnRotation;
				SpawnRotation.Yaw = 0.0f;
				SpawnRotation.Pitch = 0.0f;
				SpawnRotation.Roll = 0.0f;

			
				//spawn the pickup
				World->SpawnActor<AEnemy>(WhatToSpawn, SpawnLocation, SpawnRotation);
				SpawnDelay = FMath::FRandRange(EnemySpawnDelayLow, EnemySpawnDelayHigh);


				GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnVolume::SpawnPickup, SpawnDelay, true);

				AZIGameMode* Mode = Cast<AZIGameMode>(GetWorld()->GetAuthGameMode());
				if (Mode != NULL)
				{
					Mode->IncreaseEnemyCounter();
				}


			
		}
	}
}
