#include "SphereCharacter.h"
#include "Engine/LocalPlayer.h"
#include "MyCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

ASphereCharacter::ASphereCharacter(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

	// Components

	// Store the Character's Movement Component into the MyCharacterMovementComponent member
	MyCharacterMovementComponent = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());
	// Replicate information about the movement component to the server and all clients
	MyCharacterMovementComponent->SetIsReplicated(true);

	// Create a Spring Arm Component (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Attach the Spring Arm to the Root Component
	CameraBoom->SetupAttachment(RootComponent);
	// Rotate the Spring Arm based on the controller
	CameraBoom->bUsePawnControlRotation = true; 

	// Create a Follow Camera Component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the Camera to the end of the Spring Arm (Let the Spring Arm adjust to match the controller orientation)
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Do Not rotate the Camera based on the controller (Let the Spring Arm control the rotation)
	FollowCamera->bUsePawnControlRotation = false;
}

void ASphereCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {

			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Setup Input Action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASphereCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASphereCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASphereCharacter::SprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASphereCharacter::SprintReleased);
	}
}

void ASphereCharacter::Move(const FInputActionValue& Value) {
	
	// Input is a Vector2D (1, 0)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		// Get Controller Rotation (X, Y, Z)
		const FRotator Rotation = Controller->GetControlRotation();

		// Get Controller Yaw Rotation (0, Y, 0)
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get Forward Direction of the Controller
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get Right Direction of the Controller
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add Movement in the Forward Direction based on the Input's Y Value
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// Add Movement in the Right Direction based on the Input's X Value
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASphereCharacter::Look(const FInputActionValue& Value) {
	
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		// Add Yaw Input (X-Axis)
		AddControllerYawInput(LookAxisVector.X);

		// Add Pitch Input (Y-Axis)
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASphereCharacter::SprintPressed() {
	
	if (MyCharacterMovementComponent) {

		MyCharacterMovementComponent->SetSprintKeyDown(true);
	}
}

void ASphereCharacter::SprintReleased() {

	if (MyCharacterMovementComponent) {

		MyCharacterMovementComponent->SetSprintKeyDown(false);
	}
}
