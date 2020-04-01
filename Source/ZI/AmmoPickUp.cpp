// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickUp.h"
#include "Components/StaticMeshComponent.h"
#include "ZICharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoPickUp::AAmmoPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//All pickups start active
	AmmoPickupIsActive = true;
	//Mesh for ammo pickup
	AmmoPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoPickupMesh"));
	RootComponent = AmmoPickupMesh;

	
}

// Called when the game starts or when spawned
void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAmmoPickUp::IsActive()
{
	return AmmoPickupIsActive;
}

void AAmmoPickUp::SetActive(bool NewPickupState)
{
	AmmoPickupIsActive = NewPickupState;
}

void AAmmoPickUp::IsCollected_Implementation()
{
	AZICharacter* Player = Cast<AZICharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	Player->Ammo += 1;
	Destroy();
}