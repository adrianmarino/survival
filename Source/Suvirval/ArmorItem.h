#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "ArmorItem.generated.h"

UCLASS()
class SUVIRVAL_API AArmorItem : public AActor
{
	GENERATED_BODY()

	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------

	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage System", meta = (AllowPrivateAccess = "true"))
		FRotator RotationRate;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage System", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage System", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* TriggerCapsule;

		UPROPERTY(
			EditAnywhere, 
			BlueprintReadWrite, 
			Category = "Damage System", 
			meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0")
		)
		float Quantity;

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------

	public:	
		AArmorItem();

	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	public:	
		virtual void Tick(float DeltaTime) override;

		UFUNCTION()
		void OnOverlapBegin(
			class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	private:
		UCapsuleComponent* CreateTriggerCapsule(
			FName SubobjectName, 
			FName InCollisionProfileName, 
			float InRadius, 
			float InHalfHeight
		);
};
