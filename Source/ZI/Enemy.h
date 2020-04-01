// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ZI_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyHealth")
		float MaxEnemyHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyHealth")
		float EnemyHealth;
	UFUNCTION(BlueprintCallable, Category = "EnemyHealth")
		float GetEnemyHealth();

	UFUNCTION(BlueprintCallable, Category = "EnemyDeath")
		void Death();

	UFUNCTION(BlueprintCallable, Category = "EnemyDeath")
		void Sacrafice();

	void RandomTarget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zombie Target")
		int ZombieTarget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// true when can be picked up false when it cant
	bool EnemySpawnIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);
	
};
