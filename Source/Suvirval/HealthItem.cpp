// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"


// Sets default values
AHealthItem::AHealthItem()
{
	TriggerCapsule = CreateTriggerCapsule();
	TriggerCapsule->SetupAttachment(RootComponent);
	RootComponent = TriggerCapsule; 
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerCapsule);

	RotationRate = FRotator(0, 180, 0);

	Quantity = 0.5;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UCapsuleComponent* AHealthItem::CreateTriggerCapsule() {
	UCapsuleComponent* Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCapsule"));
	Capsule->InitCapsuleSize(100, 100);
	Capsule->SetCollisionProfileName(TEXT("Trigger"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnOverlapBegin);
	return Capsule;
}

// Called when the game starts or when spawned
void AHealthItem::BeginPlay() {
	Super::BeginPlay();
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Health Item Created!");
}

// Called every frame
void AHealthItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->AddActorLocalRotation(RotationRate * DeltaTime);
}

void AHealthItem::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
) {
	ASuvirvalCharacter* Character = Cast<ASuvirvalCharacter>(OtherActor);
	if(Character && !Character->HealthIsFull()) {
		Character->IncreaseHealth(this->Quantity);
		this->Destroy();
	}
}