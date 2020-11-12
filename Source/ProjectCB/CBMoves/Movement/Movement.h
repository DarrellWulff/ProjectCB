#pragma once

#include "CoreMinimal.h"
#include "../../CBMath/Proportion.h"

struct PROJECTCB_API Movement
{
	// TODO add other movement variables here
	
	static const float PLAYER_GROUND_SPEED;
	static const float PLAYER_AIR_SPEED;
	static const float PLAYER_ACCELERATION; // between 0 and 1
	static const float PLAYER_DECELERATION; // between 0 and 1

	static const float GHOST_SPEED;
	static const float GHOST_ACCELERATION; // between 0 and 1
	static const float GHOST_DECELERATION; // between 0 and 1

private:

	float m_inputRotationYaw;

	FRotator m_startRotation;
	FRotator m_playerRotation;

	FVector2D m_inputVelocity;
	FVector2D m_currentMovementVelocity;
	FVector2D m_currentRotationVelocity;

	float m_playerSpeed;

	void updateVelocity(FVector2D& currentVelocity, float mobility);

	bool m_movementPaused;

public:

	Movement();

	void setStartRotation(FRotator startRotation);

	FRotator getPlayerRotation();

	void updateVelocity(float mobility);

	void isGrounded(bool grounded);

	FVector getMovementVelocity(float velocityZ);

	void resetInputVelocity();
	void addInputVector(FVector inputVector);
	void setInputRotation(float inputRotationYaw);
	FVector getInputVector(float cameraRotationYaw);

	void resetMovement(float amount);
};
