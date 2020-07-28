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
#include "cEntity.h"
#include "cBullet.h"
using namespace glm;

class cPlayer : public cModel
{
private:
	bool m_bBulletFired = false;

public:
	//Use model constructor
	using cModel::cModel;

	void Rotate(float _deltaTime, cInput* _inputManager);
	void Shoot(float _deltaTime, cInput* _inputManager, std::vector<cBullet*>& m_vecBullets);
	void ResetFire();
	bool IsFired();
	cBullet* InitialiseBullet();
};

