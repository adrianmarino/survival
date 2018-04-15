#include "DamageItem.h"
#include "Damaging.h"
#include "Screen.h"

ADamageItem::ADamageItem()
{
	TriggerCapsule = CreateTriggerCapsule(TEXT("DamageTriggerCapsule"), TEXT("DamageTrigger"), 50, 50);

	TriggerCapsule->SetupAttachment(RootComponent);
	RootComponent = TriggerCapsule; 

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageMesh"));
	Mesh->SetupAttachment(TriggerCapsule);

	RotationRate = FRotator(0, 180, 0);

	PrimaryActorTick.bCanEverTick = true;
}

UCapsuleComponent* ADamageItem::CreateTriggerCapsule(
	FName SubobjectName, 
	FName InCollisionProfileName, 
	float InRadius, 
	float InHalfHeight
) {
	auto Capsule = CreateDefaultSubobject<UCapsuleComponent>(SubobjectName);
	Capsule->InitCapsuleSize(InRadius, InHalfHeight);
	Capsule->SetCollisionProfileName(InCollisionProfileName);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ADamageItem::OnOverlapBegin);
	return Capsule;
}

void ADamageItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->AddActorLocalRotation(RotationRate * DeltaTime);
}

void ADamageItem::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
) {
	IDamaging* damaging = Cast<IDamaging>(OtherActor);
	if(damaging && !damaging->LifeIsZero()) {
		Screen::Message(TEXT("Apply Damage!"));
		damaging->Damage(Quantity);
	}
}