#include "ArmorItem.h"
#include "Damaging.h"
#include "Screen.h"

AArmorItem::AArmorItem()
{
	TriggerCapsule = CreateTriggerCapsule(TEXT("ArmorTriggerCapsule"), TEXT("ArmorTrigger"), 50, 50);
	TriggerCapsule->SetupAttachment(RootComponent);
	RootComponent = TriggerCapsule; 

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmorMesh"));
	Mesh->SetupAttachment(TriggerCapsule);

	RotationRate = FRotator(0, 180, 0);

	PrimaryActorTick.bCanEverTick = true;
}

UCapsuleComponent* AArmorItem::CreateTriggerCapsule(
	FName SubobjectName, 
	FName InCollisionProfileName, 
	float InRadius,
	float InHalfHeight
) {
	auto Capsule = CreateDefaultSubobject<UCapsuleComponent>(SubobjectName);
	Capsule->InitCapsuleSize(InRadius, InHalfHeight);
	Capsule->SetCollisionProfileName(InCollisionProfileName);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AArmorItem::OnOverlapBegin);
	return Capsule;
}

void AArmorItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->AddActorLocalRotation(RotationRate * DeltaTime);
}

void AArmorItem::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
) {
	IDamaging* damaging = Cast<IDamaging>(OtherActor);
	if(damaging && !damaging->ArmorIsFull()) {
		Screen::Message(TEXT("Apply Armor!"));
		damaging->IncreaseArmor(Quantity);
		this->Destroy();
	}
}