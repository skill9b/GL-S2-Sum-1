#include "cCamera.h"

cCamera::cCamera()
{
	m_fSCR_WIDTH = 0.0f;
	m_fSCR_HEIGHT = 0.0f;
	m_fFar = 0.0f;
	m_fNear = 0.0f;
	m_fTimeElapsed = 0.0f;
	m_v3Position = vec3(4.0f, 5.0f, 5.0f);
	m_v3LookDirection = vec3(0.0f, 0.0f, 0.0f);
	m_v3UpDirection = vec3(0.0f, 1.0f, 0.0f);
}

void cCamera::Initialise(float _fWidth, float _fHeight, float _fFar, float _fNear)
{
	m_fSCR_WIDTH = _fWidth;
	m_fSCR_HEIGHT = _fHeight;
	m_fFar = _fFar;
	m_fNear = _fNear;
}

cCamera::~cCamera()
{
}

float cCamera::GetWidth()
{
	return m_fSCR_WIDTH;
}

float cCamera::GetHeight()
{
	return m_fSCR_HEIGHT;
}

float cCamera::GetFar()
{
	return m_fFar;
}

float cCamera::GetNear()
{
	return m_fNear;
}

void cCamera::SetProjectionMatrix(mat4 _m4NewMatrix)
{
	m_m4Projection = _m4NewMatrix;
}

void cCamera::SetViewMatrix()
{
	m_m4View = lookAt(m_v3Position, m_v3LookDirection, m_v3UpDirection);
}

void cCamera::SetPosition(vec3 newPosition)
{
	m_v3Position = newPosition;
}

void cCamera::MoveRelative(vec3 Translation)
{
	SetPosition(Translation + GetCamPos());
}

void cCamera::Update(float _deltaTime)
{
	//Update matrices
	SetViewMatrix();

	SetProjectionMatrix(perspective(45.0f, GetWidth() / GetHeight(), m_fNear, m_fFar));
}

mat4 cCamera::GetProjectionMatrix()
{
	return m_m4Projection;
}

mat4 cCamera::GetViewMatrix()
{
	return m_m4View;
}

mat4 cCamera::GetPVMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

vec3 cCamera::GetCamPos()
{
	return m_v3Position;
}



