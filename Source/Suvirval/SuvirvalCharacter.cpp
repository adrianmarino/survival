#include "SuvirvalCharacter.h"
#include "Screen.h"

//-----------------------------------------------------------------------------
// Constructor / Descrutor
//-----------------------------------------------------------------------------

ASuvirvalCharacter::ASuvirvalCharacter() {
	ForwardInput = .0f;
	RightInput = .0f;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	this->InitializeMovement();
	this->InitializeCamera();

	PrimaryActorTick.bCanEverTick = true;
}

ASuvirvalCharacter::~ASuvirvalCharacter() {
 	delete Health;
	delete Armor;
}

//-----------------------------------------------------------------------------
// Methods
//-----------------------------------------------------------------------------

void ASuvirvalCharacter::InitializeLevels() {
	Health = new PercentLevel(InitialHealth);
	Armor = new PercentLevel(InitialArmor);

	Armor->SetIncreaseCondition([&] { return this->Health->IsFull(); });
	Armor->SetDecreaseCondition([&] { return this->Health->IsFull(); });

	Health->SetIncreaseCondition([&] { return this->Armor->IsZero(); });
	Health->SetDecreaseCondition([&] { return this->Armor->IsZero(); });
}

void ASuvirvalCharacter::InitializeMovement() {
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

void ASuvirvalCharacter::InitializeCamera() {
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->CreationMethod = EComponentCreationMethod::Native;
	FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform , TEXT("head"));
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->RelativeLocation = FVector(5.0f, 19.0f, 0.0f);
	FollowCamera->RelativeRotation = FRotator(-90.f, 0.0f, 90.0f);
}

void ASuvirvalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASuvirvalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASuvirvalCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASuvirvalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASuvirvalCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASuvirvalCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASuvirvalCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASuvirvalCharacter::OnResetVR);
}


void ASuvirvalCharacter::OnResetVR() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASuvirvalCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location) {
	Jump();
}

void ASuvirvalCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location) {
	StopJumping();
}

void ASuvirvalCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASuvirvalCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASuvirvalCharacter::MoveForward(float Value) {
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

void ASuvirvalCharacter::MoveRight(float Value) {
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

// Called every frame
void ASuvirvalCharacter::Tick( float DeltaTime ) {
	Super::Tick(DeltaTime);
}

void ASuvirvalCharacter::BeginPlay() {
    Super::BeginPlay();
	this->InitializeLevels();
	GetWorldTimerManager().SetTimer(
		this->DamageTimer,
		[&]() {
			this->Armor->Increase();
			this->Health->Increase();
			Screen::ShowDamage(this->Health, this->Armor);
		},
		ONE_SECOND,
		LOOP_TIMER, 
		ONE_SECOND
	);
}

void ASuvirvalCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(this->DamageTimer);
}

bool ASuvirvalCharacter::LifeIsZero() {
	return Armor->IsZero() && Health->IsZero();
}

void ASuvirvalCharacter::Damage(float Quantity) {
	Armor->Drecrease(Quantity);
	Health->Drecrease(Quantity);
}

void ASuvirvalCharacter::IncreaseArmor(float Quantity) { Armor->Increase(Quantity); }

void ASuvirvalCharacter::IncreaseHealth(float Quantity) { Health->Increase(Quantity); }

bool ASuvirvalCharacter::ArmorIsFull() { return Armor->IsFull(); }

bool ASuvirvalCharacter::HealthIsFull() { return Health->IsFull(); }

float ASuvirvalCharacter::GetCurrentArmor() { return Armor->GetValue(); }

float ASuvirvalCharacter::GetCurrentHealth() { return Health->GetValue(); }


