// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZIGameMode.h"
#include "ZICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GunHUD.h"
#include "EnemySpawnVolume.h"
#include "Enemy.h"
#include "SpawnVolume.h"
#include "Church.h"
#include "Cabin.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


AZIGameMode::AZIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = AGunHUD::StaticClass();



	WaveNumber = 1;
	WaveLimit = 6;

	//set the total number of enemies to the 10
	TotalEnemiesThisWave = DEFAULT_ENEMY_NUM;

	GameOver = false;
	
}

void AZIGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundEnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnVolume::StaticClass(), FoundEnemyActors);
	

	for (auto Actor : FoundEnemyActors)
	{
		AEnemySpawnVolume* EnemySpawnVolumeActor = Cast<AEnemySpawnVolume>(Actor);
		if (EnemySpawnVolumeActor)
		{
			EnemySpawnVolumeActors.AddUnique(EnemySpawnVolumeActor);
		}
	}

	SetCurrentState(EPlayState::EPlaying);
	
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

	}
	
	
}

void AZIGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AEnemy::StaticClass(), Actors);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChurch::StaticClass(), Buildings);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACabin::StaticClass(), Buildings);

	ActiveEnemies = Actors.Num();
	ActiveBuildings = Buildings.Num();
	if (GetCurrentState() == EPlayState::EPlaying)
	{
		if (SpawnedEnemies >= TotalEnemiesThisWave)
		{
			for (AEnemySpawnVolume* Volume : EnemySpawnVolumeActors)
			{
				Volume->SetSpawnActive(false);
			}
		}

		if ((SpawnedEnemies >= TotalEnemiesThisWave) && (Actors.Num() <= 0))
		{
			SetCurrentState(EPlayState::EWaiting);
		}
	}
	if (WaveNumber == WaveLimit)
	{
		SetCurrentState(EPlayState::EWon);
	}
	if (Buildings.Num() <= 0)
	{
		SetCurrentState(EPlayState::EGameOver);
	}
	
}

void AZIGameMode::IncreaseEnemyCounter()
{
	this->SpawnedEnemies++;
}

void AZIGameMode::DecreaseBuildings()
{
	this->ActiveBuildings--;
}

void AZIGameMode::WaveWaitTimer()
{
	GetWorldTimerManager().SetTimer(WaveWaitTime, this, &AZIGameMode::StartRound, 5.0f, false);
	UE_LOG(LogTemp, Warning, TEXT("WaveWaitTimer Called"));
}

void AZIGameMode::StartRound()
{
	SetCurrentState(EPlayState::EPlaying);
	TotalEnemiesThisWave += 10;
	WaveNumber++;
	GetWorldTimerManager().ClearTimer(WaveWaitTime);
	UE_LOG(LogTemp, Warning, TEXT("Start Round Called"));
}

EPlayState AZIGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AZIGameMode::SetCurrentState(EPlayState NewState)
{
	CurrentState = NewState;

	HandleNewState(CurrentState);
}

void AZIGameMode::HandleNewState(EPlayState NewState)
{
	switch (NewState)
	{
	 //If the game is playing
	case EPlayState::EPlaying:
	{
		SpawnedEnemies = 0;

		//Enemy Spawn volume is active until the amount of zombies have spawned
		for (AEnemySpawnVolume* Volume : EnemySpawnVolumeActors)
		{
			Volume->SetSpawnActive(true);
			
		}

		
	}
	break;
	// If the game is inbetween waves
	case EPlayState::EWaiting:
	{
		//Enemy Spawn volume is inactive
		for (AEnemySpawnVolume* Volume : EnemySpawnVolumeActors)
		{
			Volume->SetSpawnActive(false);
		}
		
		WaveWaitTimer();
	
	}
	break;
	// If the game has been won
	case EPlayState::EWon:
	{
		//Enemy spawn volume is inactive
		for (AEnemySpawnVolume* Volume : EnemySpawnVolumeActors)
		{
			Volume->SetSpawnActive(false);
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
	}
	break;
	//If the game has been lost
	case EPlayState::EGameOver:
	{
		//Enemy Spawn Volume is inactive
		for (AEnemySpawnVolume* Volume : EnemySpawnVolumeActors)
		{
			Volume->SetSpawnActive(false);
		}

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
	}
	break;

	//If the game messes up
	default:
	case EPlayState::EUnkown:
	{
	
	}
	break;
	}
}