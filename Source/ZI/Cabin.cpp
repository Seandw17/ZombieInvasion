// Fill out your copyright notice in the Description page of Project Settings.

#include "Cabin.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"
#include "ZICharacter.h"
#include "Enemy.h"
#include "ZIGameMode.h"
#include "Engine/World.h"
// Sets default values
ACabin::ACabin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CabinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinMesh"));
	CabinMesh->SetRelativeScale3D(FVector(30.f, 30.f, 30.f));
	RootComponent = CabinMesh;

	CabinBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CabinBoxCollision"));
	CabinBoxCollision->InitBoxExtent(FVector(10.f, 10.f, 10.f));
	CabinBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACabin::OnOverlapBegin);
	CabinBoxCollision->SetupAttachment(CabinMesh);

	MaxCabinHealth = 100.f;
	CabinHealth = MaxCabinHealth;

	IsCabinAlive = true;
}

// Called when the game starts or when spawned
void ACabin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACabin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CabinHealth <= 0)
	{
		Destroy();
		DecreaseBuildings();
		IsCabinAlive = false;
	}

}

void ACabin::DecreaseBuildings()
{
	AZIGameMode* Mode = Cast<AZIGameMode>(GetWorld()->GetAuthGameMode());
	if (Mode != NULL)
	{
		Mode->DecreaseBuildings();
	}
}

float ACabin::GetCabinHealth()
{
	return CabinHealth;
}

void ACabin::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor == Zombie)
		{
			CabinHealth -= 20.f;
			Zombie->Sacrafice();
		}
	}
}

void ACabin::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}