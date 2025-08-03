#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SphereCharacter.generated.h"

#pragma region Forward-Declarations

class UMyCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

#pragma endregion

UCLASS()
class SPHERECUBED_API ASphereCharacter : public ACharacter {

	GENERATED_BODY()

#pragma region Alias

	// Sets Super as an alias of the Parent Class
	using Super = ACharacter;

#pragma endregion

#pragma region Public

public:

	// Sets Default Values (Constructor)
	ASphereCharacter(const FObjectInitializer& ObjectInitializer);

	// Character Movement Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UMyCharacterMovementComponent* MyCharacterMovementComponent;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called for Movement Input
	void Move(const FInputActionValue& Value);

	// Called for Looking Input
	void Look(const FInputActionValue& Value);

	// Called to Start Sprinting
	void SprintPressed();

	// Called to Stop Sprinting
	void SprintReleased();

#pragma endregion

#pragma region Private

private:
	
	// Spring Arm Component that attaches the Camera to the Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Camera Component (Character's POV)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	// Input Action: Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Input Action: Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Input Action: Look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Input Action: Sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

#pragma endregion

};
