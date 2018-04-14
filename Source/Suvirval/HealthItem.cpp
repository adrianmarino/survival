#include "HealthItem.h"
#include "Damaging.h"
#include "Screen.h"

AHealthItem::AHealthItem()
{
	TriggerCapsule = CreateTriggerCapsule(TEXT("HeathTriggerCapsule"), TEXT("HeathTrigger"), 50, 50);
	TriggerCapsule->SetupAttachment(RootComponent);
	RootComponent = TriggerCapsule; 

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeathMesh"));
	Mesh->SetupAttachment(TriggerCapsule);

	RotationRate = FRotator(0, 180, 0);

	PrimaryActorTick.bCanEverTick = true;
}

UCapsuleComponent* AHealthItem::CreateTriggerCapsule(
	FName SubobjectName, 
	FName InCollisionProfileName, 
	float InRadius, 
	float InHalfHeight
) {
	auto Capsule = CreateDefaultSubobject<UCapsuleComponent>(SubobjectName);
	Capsule->InitCapsuleSize(InRadius, InHalfHeight);
	Capsule->SetCollisionProfileName(InCollisionProfileName);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnOverlapBegin);
	return Capsule;
}

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
	IDamaging* damaging = Cast<IDamaging>(OtherActor);
	if(damaging && !damaging->HealthIsFull()) {
		Screen::Message(TEXT("Apply Health!"));
		damaging->IncreaseHealth(Quantity);
		this->Destroy();
	}
}