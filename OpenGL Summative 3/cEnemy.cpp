#include "cEnemy.h"

void cEnemy::Move(float _deltaTime)
{
	this->SetTranslate(GetTranslate() + (m_v3Velocity * _deltaTime));
}

void cEnemy::SetVelocity(vec3 newVelocity)
{
	m_v3Velocity = newVelocity;
}

