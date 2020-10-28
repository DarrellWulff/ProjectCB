#pragma once

#include "CoreMinimal.h"
#include "../../CBMath/Proportion.h"

struct PROJECTCB_API Movement
{
	static const float acceleration; // between 0 and 1
	static const float deceleration; // between 0 and 1

	FVector2D m_inputVelocity;
	FVector2D m_currentVelocity;

	Movement();

	void updateVelocity();
};
