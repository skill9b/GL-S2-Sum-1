#pragma once
#include <SOIL.h>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "ShadeLoader.h"
#include "Global.h"
#include "cModel.h"
#include "cEntity.h"
#include "cInput.h"
using namespace glm;
class cBullet : public cEntity
{
private:
	vec3 m_v3Velocity = vec3(0.0f, 0.0f, -15.0f);

public:

	float m_fRadius = 1.5f;
	void Move(float _deltaTime);
	void SetVelocity(vec3 _newVelocity);
};

