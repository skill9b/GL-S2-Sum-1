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
#include "cInput.h"
using namespace glm;
class cEnemy : public cModel
{
private:
	vec3 m_v3Velocity = vec3(0.0f, 0.0f, 2.5f);

public:
	//Use model constructor
	using cModel::cModel;

	void Move(float _deltaTime);
	void SetVelocity(vec3 newVelocity);
};

