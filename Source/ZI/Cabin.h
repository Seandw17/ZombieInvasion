// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cabin.generated.h"

UCLASS()
class ZI_API ACabin : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CabinBoxCollision", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CabinBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CabinMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CabinMesh;
	
public:	
	// Sets default values for this actor's properties
	ACabin();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsCabinAlive")
		bool IsCabinAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CabinHealth")
		float MaxCabinHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CabinHealth")
		float CabinHealth;

	UFUNCTION(BlueprintCallable, Category = "CabinHealth")
		float GetCabinHealth();

	void DecreaseBuildings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE class UStaticMeshComponent* GetCabinMesh() const { return CabinMesh; }
	
};
