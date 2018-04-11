// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthItem.generated.h"

UCLASS()
class SUVIRVAL_API AHealthItem : public AActor
{
	GENERATED_BODY()

	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------
	
	public:
		
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Propeties")
		FRotator RotationRate;
			
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Propeties")
		USceneComponent* SceneComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Propeties")
		UStaticMeshComponent* Mesh;


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

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
};
