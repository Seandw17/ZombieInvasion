// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZIGameMode.generated.h"

//Enum to store current state of gameplay
UENUM(BlueprintType)
enum class EPlayState : uint8
{
	EPlaying,
	EWaiting,
	EWon,
	EGameOver,
	EUnkown

};

UCLASS(minimalapi)
class AZIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZIGameMode();

	//Returns the current playing state
	UFUNCTION(BlueprintPure)
	EPlayState GetCurrentState() const;
	//Sets a new playing state
	UFUNCTION(BlueprintCallable)
		void SetCurrentState(EPlayState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveNumer")
		int WaveNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveNumer")
		int WaveLimit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingBool")
		bool GameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingBool")
		bool IsChurchAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingBool")
		bool IsCabinAlive;

	UFUNCTION(BlueprintCallable, Category = "Buildings")
		void DecreaseBuildings();

	void IncreaseEnemyCounter();

	

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Counter")
		int ActiveEnemies;

	UPROPERTY(BlueprintReadOnly, Category = "Building Counter")
	int ActiveBuildings;

	FTimerHandle WaveWaitTime;
	FTimerHandle WaveTime;

	


private:
	//Keeps track of the current playing state
	EPlayState CurrentState;

	TArray<class AEnemySpawnVolume*> EnemySpawnVolumeActors;
	
	TArray<AActor*> Buildings;

	const int DEFAULT_ENEMY_NUM = 5;

	int SpawnedEnemies;
	int TotalEnemiesThisWave;
	
	

	//Handle any function calls that rely upon changing the playing state of our game
	void HandleNewState(EPlayState NewState);

	//Handling the switch between rounds
	void StartRound();
	void WaveWaitTimer();
};



