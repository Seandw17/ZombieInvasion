// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class ZI_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

	//Box Component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	//Find a random point within the BoxComponent
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();
	
	//This function toggles whether or not the spawn volume spawns
	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SetSpawnActive(bool bShouldSpawn);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The Pickup to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AAmmoPickUp> WhatToSpawn;

	
	

private:

	void SpawnPickup();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns WhereToSpawn Subobject
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }
	
	
};
