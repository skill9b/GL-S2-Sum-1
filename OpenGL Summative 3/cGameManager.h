#pragma once
#include "cMenu.h"
#include "cLevel.h"
#include "ShadeLoader.h"
#include "cCamera.h"
#include "cEntity.h"
#include "cCubeMap.h"

class cGameManager
{
public:
	cGameManager(); //Constructor
	~cGameManager(); //Destructor
	void Initialise(float _deltaTime);
	void Update(float _deltaTime);
	void Render();
	cInput* GetInputManager();
	cEntity* InitialiseCube(string filedir);

private:
	State m_CurrentState;
	cMenu* m_pMainMenu;
	cLevel* m_pLevelOne;
	cCamera* m_pCamera;
	GLuint m_gliReflectionProgram;
	GLuint m_gliTestProgram;
	float m_fScreenWidth, m_fScreenHeight;
	cEntity* m_pBackground;
	cInput* m_pInputManager;
	cCubeMap* m_pCubeMap;

	cEntity* m_pCube;
	cEntity* m_pScaledCube;

};
