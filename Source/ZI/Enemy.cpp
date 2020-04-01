// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ZICharacter.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxEnemyHealth = 100.f;
	EnemyHealth = MaxEnemyHealth;

	EnemySpawnIsActive = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	RandomTarget();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyHealth <= 0)
	{
		Death();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::GetEnemyHealth()
{
	return EnemyHealth;
}

void AEnemy::Death()
{
	AZICharacter* Player = Cast<AZICharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Player->PlayerKillCount += 1;
	Destroy();
}

void AEnemy::Sacrafice()
{
	Destroy();
}

bool AEnemy::IsActive()
{
	return EnemySpawnIsActive;
}

void AEnemy::SetActive(bool NewPickupState)
{
	EnemySpawnIsActive = NewPickupState;
}

void AEnemy::RandomTarget()
{
	ZombieTarget = FMath::FRandRange(0, 1);
}