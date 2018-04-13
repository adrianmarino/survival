// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "SuvirvalCharacter.h"
#include "Screen.h"
#include "HealthItem.generated.h"

UCLASS()
class SUVIRVAL_API AHealthItem : public AActor
{
	GENERATED_BODY()

	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------

	protected:
	
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Propeties", meta = (AllowPrivateAccess = "true"))
		FRotator RotationRate;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Propeties", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger Capsule", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* TriggerCapsule;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = "true"))
		float Quantity;

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
		
	public:	
		// Sets default values for this actor's properties
		AHealthItem();

	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	public:	
		// Called every frame
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

		UFUNCTION()
		void OnOverlapEnd(
			class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex
		);

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:
		UCapsuleComponent* CreateTriggerCapsule();
};
