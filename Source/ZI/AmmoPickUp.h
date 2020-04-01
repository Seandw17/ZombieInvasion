// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickUp.generated.h"

UCLASS()
class ZI_API AAmmoPickUp : public AActor
{
	GENERATED_BODY()

		//Mesh for the ammo pick up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AmmoPickupMesh;

public:	
	// Sets default values for this actor's properties
	AAmmoPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// true when can be picked up false when it cant
	bool AmmoPickupIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetAmmoPickupMesh() const { return AmmoPickupMesh; }
	
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	//Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
		void IsCollected();
	virtual void IsCollected_Implementation();
};
