#include "Movement.h"

const float Movement::acceleration = 1.0f / 8;
const float Movement::deceleration = Movement::acceleration * 2;

Movement::Movement()
{
	this->m_inputVelocity.X = 0.0f;
	this->m_inputVelocity.Y = 0.0f;

	this->m_currentVelocity.X = 0;
	this->m_currentVelocity.Y = 0;
}

void Movement::updateVelocity()
{
	FVector2D diff = this->m_inputVelocity - this->m_currentVelocity;
	float diffMag = diff.Size();
	float accProp = FVector2D::DotProduct(diff.GetSafeNormal(), this->m_currentVelocity.GetSafeNormal());
	accProp = (accProp + 1) / 2;
	Proportion accProportion(accProp);
	float accAmount = accProportion.getProportion(Movement::acceleration, Movement::deceleration);
	if (accAmount >= diffMag)
		this->m_currentVelocity = this->m_inputVelocity;
	else
		this->m_currentVelocity += diff * (accAmount / diffMag);
}
