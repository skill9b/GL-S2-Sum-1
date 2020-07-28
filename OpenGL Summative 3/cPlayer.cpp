#include "cPlayer.h"
#include "glm/ext.hpp"

void cPlayer::Rotate(float _deltaTime, cInput* _inputManager)
{
	if (_inputManager->KeyState['d'] == InputState::INPUT_DOWN_FIRST)
	{
		//Rotate right 
		this->SetRotateAngle(this->GetAngle() - ANGLE_CHANGE * _deltaTime);


	}
	else if(_inputManager->KeyState['a'] == InputState::INPUT_DOWN_FIRST)
	{
		//Rotate left
		this->SetRotateAngle(this->GetAngle() + ANGLE_CHANGE * _deltaTime);

	}

	//Limit angle
	float angleLimit = 45.0f;

	if ((this->GetAngle() <= -angleLimit))
	{
		this->SetRotateAngle(-angleLimit);
	}
	if ((this->GetAngle() >= angleLimit))
	{
		this->SetRotateAngle(angleLimit);
	}
}

void cPlayer::Shoot(float _deltaTime, cInput* _inputManager, std::vector<cBullet*>& _vecBullets)
{
	if (_inputManager->KeyState[' '] == InputState::INPUT_DOWN_FIRST && m_bBulletFired == false)
	{
		//Spawn
		cBullet* _bullet = InitialiseBullet();

		float angle = this->GetAngle();
		vec3 Shootpoint;

		//Calculate z
		float z = -7.0f * cosf(radians(angle));
		Shootpoint.z = z;

		//Calculate x
		float x = -7.0f * sinf(radians(angle));
		Shootpoint.x = x;

		//Calculate y
		Shootpoint.y = 0;

		_bullet->SetVelocity(Shootpoint);
		_bullet->SetTranslation(Shootpoint);

		_vecBullets.push_back(_bullet);
		//Limit fire rate
		m_bBulletFired = true;
	}

}

cBullet* cPlayer::InitialiseBullet()
{
	cBullet* m_pBall = new cBullet();

	//Sprite parameters
	float BallFrames = 12.0f;
	int BallFPS = 12;
	
	//Size
	float BallWidth = 12.0f;
	float BallHeight = 12.0f;

	//Vertices
	vector <Vertex2D> vecBallRectVertices;
	Vertex2D Ballv1;
	Ballv1.Position = vec3(-BallWidth, BallHeight, 0.0f);
	Ballv1.Color = vec3(0.0f, 1.0f, 0.0f);
	Ballv1.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Ballv2;
	Ballv2.Position = vec3(-BallWidth, -BallHeight, 0.0f);
	Ballv2.Color = vec3(1.0f, 0.0f, 0.0f);
	Ballv2.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Ballv3;
	Ballv3.Position = vec3(BallWidth, -BallHeight, 0.0f);
	Ballv3.Color = vec3(1.0f, 1.0f, 0.0f);
	Ballv3.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Ballv4;
	Ballv4.Position = vec3(BallWidth, BallHeight, 0.0f);
	Ballv4.Color = vec3(0.0f, 0.0f, 1.0f);
	Ballv4.TexCoords = vec2(1.0f, 0.0f);

	vecBallRectVertices.push_back(Ballv1);
	vecBallRectVertices.push_back(Ballv2);
	vecBallRectVertices.push_back(Ballv3);
	vecBallRectVertices.push_back(Ballv4);

	//Indices
	vector <GLuint > vecBallRectIndices;
	vecBallRectIndices.push_back(0);
	vecBallRectIndices.push_back(1);
	vecBallRectIndices.push_back(2);
	vecBallRectIndices.push_back(0);
	vecBallRectIndices.push_back(2);
	vecBallRectIndices.push_back(3);

	string strBallImage = "Resources/Textures/NewBall.png";
	int fBallFrames = 1;

	//Model
	vec3 v3BallTranslation = vec3(0.0f, -100.0f, 0.0f);
	vec3 v3BallRotation = vec3(0.0f, 0.0f, 1.0f);
	float fBallRotAngle = -45.0f;
	vec3 v3BallScale = vec3(1.0f, 1.0f, 1.0f);
	float fBallScaleFactor = 1.0f;

	m_pBall->Intitialise(vecBallRectVertices, vecBallRectIndices, fBallFrames, strBallImage,
		v3BallTranslation, v3BallRotation, fBallRotAngle,
		v3BallScale, fBallScaleFactor, BallFPS, BallWidth, BallHeight);

	return m_pBall;
}

void cPlayer::ResetFire()
{

	m_bBulletFired = false;
}

bool cPlayer::IsFired()
{
	return m_bBulletFired;
}
