// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Church.generated.h"

UCLASS()
class ZI_API AChurch : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChurchCollision", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ChurchSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChurchMesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Churchmesh;

public:	
	// Sets default values for this actor's properties
	AChurch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsChurchAlive?")
		bool IsAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChurchHealth")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChurchHealth")
		float Health;

	UFUNCTION(BlueprintCallable, Category = "ChurchHealth")
		float GetHealth();

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

	FORCEINLINE class USphereComponent* GetChurchSphereCollision() const { return ChurchSphereCollision; }
	
	
};
