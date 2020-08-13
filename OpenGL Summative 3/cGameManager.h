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
	void SwitchLevel(float _deltaTime);
	void ResetCurrentLevel(float _deltaTime);
	void CheckInput(float _deltaTime);
	void ToggleStencilOutline();
	void ToggleScissor();
	void CameraMove(float _deltaTime);
	void MousePick(float _deltaTime);
	bool RayEntityIntersect(cEntity* Object);
	bool ToggleCull();
	bool ToggleWireFrame();

private:
	cMenu* m_pMainMenu;
	cLevel* m_pLevelOne;
	
	///Objects
	cCubeMap* m_pCubeMap;

	//Level One
	cEntity* m_pCube;
	cEntity* m_pScaledCube;

	//Level Two
	cEntity* m_pTransparentCube;
	cEntity* m_pWater;
	cEntity* m_pCamForwardButton;
	cEntity* m_pCamBackwardButton;


	//Managers
	cCamera* m_pCamera;
	State m_CurrentState;
	cInput* m_pInputManager;

	//Utility
	float m_fScreenWidth, m_fScreenHeight;
	GLuint m_gliBasicProgram;
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

	//Back Cullng
	bool CullKeyPressed;

	//Antialiasing
	bool WireKeyPressed;

};
