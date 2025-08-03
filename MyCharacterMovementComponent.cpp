#include "MyCharacterMovementComponent.h"
#include "GameFramework/Character.h"

FNetworkPredictionData_Client* UMyCharacterMovementComponent::GetPredictionData_Client() const {

	if (ClientPredictionData == nullptr) {

		// Return our custom client prediction data class instead
		UMyCharacterMovementComponent* MutableThis = const_cast<UMyCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SphereCharacter(*this);
	}

	return ClientPredictionData;
}

void UMyCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags) {

	/*
		FLAG_JumpPressed	= 0x01,	// Jump pressed
		FLAG_WantsToCrouch	= 0x02,	// Wants to crouch
		FLAG_Reserved_1		= 0x04,	// Reserved for future use by Unreal Engine Team
		FLAG_Reserved_2		= 0x08,	// Reserved for future use by Unreal Engine Team
		// Remaining bit masks are available for custom flags.
		FLAG_Custom_0		= 0x10, // Wants to Sprint
		FLAG_Custom_1		= 0x20,
		FLAG_Custom_2		= 0x40,
		FLAG_Custom_3		= 0x80,
	*/

	Super::UpdateFromCompressedFlags(Flags);

	// Set 'WantsToSprint' to the value stored at 'FLAG_Custom_0'
	WantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UMyCharacterMovementComponent::SetSprintKeyDown(bool Sprinting) {

	// Set SprintKeyDown to whatever is passed through the parameters
	SprintKeyDown = Sprinting;

}

void UMyCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) {

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking) {

		if (SprintKeyDown) {

			if (true) {

				WantsToSprint = true;
			}
			else {

				WantsToSprint = false;
			}
		}
		else {

			WantsToSprint = false;
		}
	}
}

float UMyCharacterMovementComponent::GetMaxSpeed() const {

	switch (MovementMode) {

	case MOVE_Walking:

	case MOVE_NavWalking:

		if (IsCrouching()) {

			return MaxWalkSpeedCrouched;
		}
		else {

			if (WantsToSprint) {

				return Sprint_MaxWalkSpeed;
			}

			return Walk_MaxWalkSpeed;
		}

	case MOVE_Falling:

		return Walk_MaxWalkSpeed;

	case MOVE_Swimming:

		return MaxSwimSpeed;

	case MOVE_Flying:

		return MaxFlySpeed;

	case MOVE_Custom:

		return MaxCustomMovementSpeed;

	case MOVE_None:

	default:

		return 0.f;
	}
}

void FSavedMove_SphereCharacter::Clear() {

	Super::Clear();

	// Clear all values
	SavedWantsToSprint = 0;
}

uint8 FSavedMove_SphereCharacter::GetCompressedFlags() const {

	/*
		FLAG_JumpPressed	= 0x01,	// Jump pressed
		FLAG_WantsToCrouch	= 0x02,	// Wants to crouch
		FLAG_Reserved_1		= 0x04,	// Reserved for future use by Unreal Engine Team
		FLAG_Reserved_2		= 0x08,	// Reserved for future use by Unreal Engine Team
		// Remaining bit masks are available for custom flags.
		FLAG_Custom_0		= 0x10, // Wants to Sprint
		FLAG_Custom_1		= 0x20,
		FLAG_Custom_2		= 0x40,
		FLAG_Custom_3		= 0x80,
	*/

	uint8 Result = Super::GetCompressedFlags();

	// If True, flip the bit located at FLAG_Custom_0 (0x10) in Result.
	if (SavedWantsToSprint) {

		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool FSavedMove_SphereCharacter::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const {

	FSavedMove_SphereCharacter* NewMove = static_cast<FSavedMove_SphereCharacter*>(NewMovePtr.Get());

	// As an optimization, check if the engine can combine saved moves.
	if (SavedWantsToSprint != NewMove->SavedWantsToSprint) {

		return false;
	}

	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

void FSavedMove_SphereCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) {

	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UMyCharacterMovementComponent* charMov = Cast<UMyCharacterMovementComponent>(Character->GetCharacterMovement());
	if (charMov) {

		// Copy values into the saved move
		SavedWantsToSprint = charMov->WantsToSprint;
	}
}

void FSavedMove_SphereCharacter::PrepMoveFor(class ACharacter* Character) {

	Super::PrepMoveFor(Character);

	UMyCharacterMovementComponent* charMov = Cast<UMyCharacterMovementComponent>(Character->GetCharacterMovement());
	if (charMov) {

		// Copy values out of the saved move
		charMov->WantsToSprint = SavedWantsToSprint;
	}
}

FNetworkPredictionData_Client_SphereCharacter::FNetworkPredictionData_Client_SphereCharacter(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement) {

}

FSavedMovePtr FNetworkPredictionData_Client_SphereCharacter::AllocateNewMove() {

	return FSavedMovePtr(new FSavedMove_SphereCharacter());
}
