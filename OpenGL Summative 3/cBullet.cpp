#include "cBullet.h"

void cBullet::Move(float _deltaTime)
{
	this->SetTranslation(GetTranslate() + (m_v3Velocity * _deltaTime));
}

void cBullet::SetVelocity(vec3 _newVelocity)
{
	m_v3Velocity = _newVelocity;
}
