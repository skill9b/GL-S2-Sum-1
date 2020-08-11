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
	cEntity* InitialiseQuad(string filedir);

	void RenderCurrentLevel();
	void UpdateCurrentLevel(float _deltaTime);
	void SwitchLevel();
	void ResetCurrentLevel();
	void CheckInput();
	void ToggleStencilOutline();
	void ToggleScissor();
	void CameraMove(float _deltaTime);


private:
	cMenu* m_pMainMenu;
	cLevel* m_pLevelOne;
	
	///Objects
	cEntity* m_pBackground;
	cCubeMap* m_pCubeMap;

	//Level One
	cEntity* m_pCube;
	cEntity* m_pScaledCube;

	//Level Two
	cEntity* m_pTransparentCube;
	cEntity* m_pWater;

	//Managers
	cCamera* m_pCamera;
	State m_CurrentState;
	cInput* m_pInputManager;

	//Utility
	float m_fScreenWidth, m_fScreenHeight;
	GLuint m_gliReflectionProgram;
	GLuint m_gliTestProgram;
	bool IsFirstLevel;
	float CamSpeed;

	//Stencil 
	bool StencilOn;
	bool StencilKeyPressed;

	//Scissor
	bool ScissorKeyPressed;
	bool ScissorDisabled;

	//Switch Level/Restart
	bool RestartKeyPressed;

	//Back Bullng
	bool BullKeyPressed;

	//Antialiasing
	bool AAKeyPressed;

};
