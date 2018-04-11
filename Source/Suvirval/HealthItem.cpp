// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"


// Sets default values
AHealthItem::AHealthItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	RotationRate = FRotator(0, 180, 0);
}

// Called when the game starts or when spawned
void AHealthItem::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AHealthItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->AddActorLocalRotation(RotationRate * DeltaTime);
}

