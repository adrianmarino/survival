// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LogMacros.h"
#include "SuvirvalCharacter.generated.h"

UCLASS(config=Game)
class ASuvirvalCharacter : public ACharacter
{
	GENERATED_BODY()
	
	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------

	private:

		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

		/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float  ForwardInput;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float  RightInput;

		/** Follow camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageSystem", meta = (AllowPrivateAccess = "true"))
		float ArmorLevel;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageSystem", meta = (AllowPrivateAccess = "true"))
		float HealthLevel;

		FTimerHandle DamageTimer;

	//-----------------------------------------------------------------------------
	// Constructors
	//-----------------------------------------------------------------------------

	public:
		ASuvirvalCharacter();

	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	public:
		virtual void Tick( float DeltaSeconds ) override;
		
		virtual void BeginPlay() override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		/** Returns FollowCamera subobject **/
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

		UFUNCTION()
		void OnOverlapBegin(
			UPrimitiveComponent* OverlappedComp, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep,
			const FHitResult& SweepResult
		);

		UFUNCTION()
		void OnOverlapEnd(
			UPrimitiveComponent* OverlappedComp, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex
		);

		void IncreaseArmor(float Quantity);

		void IncreaseHealth(float Quantity);

		bool ArmorIsFull();

		bool HealthIsFull();

	protected:

		/** Resets HMD orientation in VR. */
		void OnResetVR();

		/** Called for forwards/backward input */
		void MoveForward(float Value);

		/** Called for side to side input */
		void MoveRight(float Value);

		/** 
		 * Called via input to turn at a given rate. 
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void TurnAtRate(float Rate);

		/**
		 * Called via input to turn look up/down at a given rate. 
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void LookUpAtRate(float Rate);

		/** Handler for when a touch input begins. */
		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

		/** Handler for when a touch input stops. */
		void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

		// APawn interface
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		// End of APawn interface
};
