#pragma once

#include <glew.h>
#include <freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "cCamera.h"

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Render();
	GLuint GetVAO();
	GLuint GetIndice();

private:
	GLuint m_gluiVAO;

	GLuint m_gluiIndiceCount;
	int m_iDrawType;
};

