#pragma once
#include <glew.h>
#include <freeglut.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class cCamera
{
private:
	float m_fTimeElapsed;
	float m_fSCR_WIDTH;
	float m_fSCR_HEIGHT;
	float m_fFar;	
	float m_fNear;
	mat4 m_m4Projection;
	mat4 m_m4View;
	vec3 m_v3Position;
	vec3 m_v3LookDirection;
	vec3 m_v3UpDirection;


public:
	cCamera(); //Constructor
	void Initialise(float _fWidth, float _fHeight, float _fFar, float _fNear);
	~cCamera(); //Destructor

	//Get
	float GetWidth();
	float GetHeight();
	float GetFar();
	float GetNear();
	mat4 GetProjectionMatrix();
	mat4 GetViewMatrix();
	mat4 GetPVMatrix();
	vec3 GetCamPos();

	//Set
	void SetProjectionMatrix(mat4 _m4NewMatrix);
	void SetViewMatrix(); //Calculates view matrix

	void Update(float _deltaTime);
};

