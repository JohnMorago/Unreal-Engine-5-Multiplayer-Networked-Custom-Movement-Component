#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

UCLASS(BlueprintType)
class SPHERECUBED_API UMyCharacterMovementComponent : public UCharacterMovementComponent {

	GENERATED_BODY()

#pragma region Alias

	// Sets Super as an alias of the Parent Class
	using Super = UCharacterMovementComponent;

#pragma endregion

#pragma region Friends

	friend class FSavedMove_SphereCharacter;

#pragma endregion

#pragma region Public

public:

	// Get our custom Client Prediction Data class instead of the Default one
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Update variables from Compressed Flags
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	// Is the Sprint Key down or not?
	void SetSprintKeyDown(bool Sprinting);

	// Automatically called at the end of every PerformMove() (Executes regardless of what MovementMode the character is in)
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	// Returns the Max Speed of the Character
	virtual float GetMaxSpeed() const override;

#pragma endregion

#pragma region Private

private:

	// Is the Sprint key pressed down or not?
	bool SprintKeyDown = false;

	// Compressed Flag
	uint8 WantsToSprint : 1;

	// Max Speed while Walking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Variables", meta = (AllowPrivateAccess = "true"))
	float Walk_MaxWalkSpeed = 600.0f;

	// Max Speed while Sprinting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement|Variables", meta = (AllowPrivateAccess = "true"))
	float Sprint_MaxWalkSpeed = 1200.0f;

#pragma endregion

};

class FSavedMove_SphereCharacter : public FSavedMove_Character {

#pragma region Alias

	// Sets Super as an alias of the Parent Class
	using Super = FSavedMove_Character;

#pragma endregion

#pragma region Public

public:

	// Resets all saved variables.
	virtual void Clear() override;

	// Store boolean information in the compressed flags.  (uint8 = 1 byte = 8 bits)  Each bit can be used to store 1 bool value.
	virtual uint8 GetCompressedFlags() const override;

	// This is used to check whether or not two moves can be combined into one.
	// Basically you just check to make sure that the saved variables are the same.
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const override;

	// Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;

	// Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;

#pragma endregion

#pragma region Private

private:

	// Compressed Flag
	uint8 SavedWantsToSprint : 1;

#pragma endregion

};

class FNetworkPredictionData_Client_SphereCharacter : public FNetworkPredictionData_Client_Character {

#pragma region Alias

	// Sets Super as an alias of the Parent Class
	using Super = FNetworkPredictionData_Client_Character;

#pragma endregion

#pragma region Public

public:

	// Constructor
	FNetworkPredictionData_Client_SphereCharacter(const UCharacterMovementComponent& ClientMovement);

	// Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;

#pragma endregion

};
