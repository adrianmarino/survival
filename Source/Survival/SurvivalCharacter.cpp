#include "SurvivalCharacter.h"
#include "Screen.h"
#include "Blueprint/UserWidget.h"

//-----------------------------------------------------------------------------
// Constructor / Destructor
//-----------------------------------------------------------------------------

ASurvivalCharacter::ASurvivalCharacter()
{
	ForwardInput = .0f;
	RightInput = .0f;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	this->InitializeMovement();
	this->InitializeCamera();
	this->InitializePainSound();
	this->InitializeLevels();
}

ASurvivalCharacter::~ASurvivalCharacter()
{
	delete Health;
	delete Armor;
}

//-----------------------------------------------------------------------------
// Methods
//-----------------------------------------------------------------------------

void ASurvivalCharacter::InitializePainSound()
{
	// Create an audio component, the audio component wraps the Cue, and allows us to ineract with
	// it, and its parameters from code.
	PainAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PainAudioComp"));
	// I don't want the sound playing the moment it's created.
	PainAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	// I want the sound to follow the pawn around, so I attach it to the Pawns root.
	PainAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// I want the sound to come from slighty in front of the pawn.
	PainAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

void ASurvivalCharacter::InitializeLevels()
{
	Health = new PercentLevel(InitialHealth);
	Armor = new PercentLevel(InitialArmor);

	Armor->SetIncreaseCondition([&] { return this->Health->IsFull(); });
	Armor->SetDecreaseCondition([&] { return this->Health->IsFull(); });

	Health->SetIncreaseCondition([&] { return this->Armor->IsZero(); });
	Health->SetDecreaseCondition([&] { return this->Armor->IsZero(); });
}

void ASurvivalCharacter::InitializeMovement()
{
	// set our turn rates for input
	BaseTurnRate = 25.f;
	BaseLookUpRate = 25.f;


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->NavAgentProps.AgentRadius = 42.000000f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 100.000000f;
}

void ASurvivalCharacter::InitializeCamera()
{
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->CreationMethod = EComponentCreationMethod::Native;
	FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("head"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->RelativeLocation = FVector(-1.0f, -20.0f, 40.0f);
	FollowCamera->RelativeRotation = FRotator(-90.f, 90.0f, 0.0f);
}

void ASurvivalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASurvivalCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASurvivalCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivalCharacter::OnResetVR);
}

void ASurvivalCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivalCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASurvivalCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASurvivalCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::MoveForward(float Value)
{
	ForwardInput = Value;

	if ((Controller != NULL) && (Value != 0.0f)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASurvivalCharacter::MoveRight(float Value)
{
	RightInput = Value;

	if ( (Controller != NULL) && (Value != 0.0f) ) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->InitializeLevels();
	GetWorldTimerManager().SetTimer(
	    this->DamageTimer,
	[&]() {
		Armor->Increase();
		Health->Increase();
	},
	ONE_SECOND,
	LOOP_TIMER,
	ONE_SECOND
	);
}

void ASurvivalCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(this->DamageTimer);
}

bool ASurvivalCharacter::LifeIsZero()
{
	return Armor->IsZero() && Health->IsZero();
}

void ASurvivalCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PainAudioCue->IsValidLowLevelFast()) {
		PainAudioComponent->SetSound(PainAudioCue);
	}
}

void ASurvivalCharacter::ShowDamageScreen()
{
	if (!DamageScreen) return;

	// Create the widget and store it.
	DamageScreenWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), DamageScreen);

	// now you can use the widget directly since you have a referance for it.
	// Extra check to  make sure the pointer holds the widget.
	if (DamageScreenWidget) {
		//let add it to the view port
		DamageScreenWidget->AddToViewport();
	}

	// Widget-Destroy();
}

void ASurvivalCharacter::Damage(float Quantity)
{
	PainAudioComponent->Play();
	ShowDamageScreen();
	Armor->Decrease(Quantity);
	Health->Decrease(Quantity);
}

void ASurvivalCharacter::IncreaseArmor(float Quantity)
{
	Armor->ForceIncrease(Quantity);
}

void ASurvivalCharacter::IncreaseHealth(float Quantity)
{
	Health->ForceIncrease(Quantity);
}

bool ASurvivalCharacter::ArmorIsFull()
{
	return Armor->IsFull();
}

bool ASurvivalCharacter::HealthIsFull()
{
	return Health->IsFull();
}

float ASurvivalCharacter::GetCurrentArmor()
{
	return Armor->GetValue();
}

float ASurvivalCharacter::GetCurrentHealth()
{
	return Health->GetValue();
}
