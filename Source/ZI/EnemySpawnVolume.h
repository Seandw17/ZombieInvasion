// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

UCLASS()
class ZI_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()

	//Box Component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

public:	
	// Sets default values for this actor's properties
	AEnemySpawnVolume();

	//Find a random point within the BoxComponent
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	//The Pickup to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AEnemy> WhatToSpawn;


	FTimerHandle SpawnTimer;

	//Minimum Spawn Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float EnemySpawnDelayLow;

	//Maximum Spawn Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float EnemySpawnDelayHigh;

	//Maximum Spawn Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		int EnemySpawnLimit;

	//This function toggles whether or not the spawn volume spawns
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawnActive(bool bShouldSpawn);

private:

	void SpawnPickup();
	//current spawn delay
	float SpawnDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns WhereToSpawn Subobject
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }
	
};
