#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LogMacros.h"
#include "PercentLevel.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Damaging.h"
#include "SuvirvalCharacter.generated.h"


UCLASS(config=Game)
class ASuvirvalCharacter : public ACharacter, public IDamaging
{
	GENERATED_BODY()

	//-----------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------

	private:
		const bool 	LOOP_TIMER = true;
		const float ONE_SECOND = 1.0f;

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

		UPROPERTY(
			EditAnywhere, 
			BlueprintReadWrite, 
			Category = "Damage System", 
			meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0")
		)
		float InitialArmor;

		UPROPERTY(
			EditAnywhere, 
			BlueprintReadWrite, 
			Category = "Damage System", 
			meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0")
		)
		float InitialHealth;

		PercentLevel* Armor;

		PercentLevel* Health;

		FTimerHandle DamageTimer;

	//-----------------------------------------------------------------------------
	// Constructors / Destructor
	//-----------------------------------------------------------------------------

	public:
		ASuvirvalCharacter();
		~ASuvirvalCharacter();

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

		virtual void Damage(float Quantity) override;

		virtual void IncreaseArmor(float Quantity) override;

		virtual void IncreaseHealth(float Quantity) override;
		
		virtual bool LifeIsZero() override;

		virtual bool ArmorIsFull() override;

		virtual bool HealthIsFull() override;

		UFUNCTION(BlueprintPure, Category="Damage System")
		virtual float GetCurrentArmor() override;

		UFUNCTION(BlueprintPure, Category="Damage System")
		virtual float GetCurrentHealth() override;

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
	
	private:
		void InitializeLevels();

		void InitializeMovement();
		
		void InitializeCamera();
};
