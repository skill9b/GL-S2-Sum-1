#include "cGameManager.h"
#include <gtx/intersect.hpp>

cGameManager::cGameManager()
{
	m_CurrentState = State::Menu;
	m_pInputManager = new cInput();
	m_pMainMenu = new cMenu();
	m_pLevelOne = new cLevel();
	m_pCamera = new cCamera();
	m_pBackground = new cEntity();
	m_pCubeMap = 0;


	m_pCube = new cEntity();
	m_pScaledCube = new cEntity();
	m_pWater = new cEntity();


	m_gliReflectionProgram = 0;
	m_gliTestProgram = 0;
	m_fScreenWidth = SCR_WIDTH;
	m_fScreenHeight = SCR_HEIGHT;

	CamSpeed = 50.0f;
	IsFirstLevel = false;
	StencilOn = false;
	StencilKeyPressed = false;
	ScissorKeyPressed = false;
	RestartKeyPressed = false;
	ScissorDisabled = false;
}

cGameManager::~cGameManager()
{
	delete m_pLevelOne;
	delete m_pMainMenu;
	delete m_pCamera;
	delete m_pInputManager;


	delete m_pCube;
	delete m_pScaledCube;
	delete m_pWater;

	m_pLevelOne = 0;
	m_pCube = 0;
	m_pScaledCube = 0;
	m_pMainMenu = 0;
	m_pCamera = 0;
	m_pInputManager = 0;
}

void cGameManager::Initialise(float _deltaTime)
{
	//Initilise member variables
	//m_pLevelOne->Initialise(m_pCamera);
	m_pMainMenu->Initialise();
	m_pCamera->Initialise(SCR_WIDTH, SCR_HEIGHT, 10000.0f, 0.1f);
	m_pCubeMap = new cCubeMap(m_pCamera);


	m_pCube = InitialiseCube("Resources/Textures/YellowCube.png");
	m_pScaledCube = InitialiseCube("Resources/Textures/RedOutline.png");
	m_pScaledCube->SetScale(vec3(1.0f, 1.0f, 1.0f), 1.2f);

	m_pTransparentCube = InitialiseCube("Resources/Textures/TransparentCube.png");
	m_pWater = InitialiseQuad("Resources/Textures/Water.png");
	m_pWater->SetScale(vec3(2.0f, 1.0f, 2.0f), 1.0f);
	m_pWater->SetTranslation(m_pWater->GetTranslate() + vec3(0.0f, 0.25f, 0.0f));

	//Program
	m_gliReflectionProgram = ShaderLoader::CreateProgram("Resources/Shaders/Reflection.vs",
		"Resources/Shaders/Reflection.fs");

	m_gliTestProgram = ShaderLoader::CreateProgram("Resources/Shaders/Fog.vs",
		"Resources/Shaders/Fog.fs");

	Update(_deltaTime);
}

void cGameManager::Update(float _deltaTime)
{
	//Update persistant objects	
	CameraMove(_deltaTime);
	MousePick();
	m_pCamera->Update(_deltaTime);
	m_pCubeMap->Update();
	CheckInput();
	UpdateCurrentLevel(_deltaTime);
	
	glutPostRedisplay();
}

void cGameManager::Render()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Render persistant objects
	m_pCubeMap->Render();

	RenderCurrentLevel();

	glutSwapBuffers();
}

cInput* cGameManager::GetInputManager()
{
	return m_pInputManager;
}

void cGameManager::RenderCurrentLevel()
{
	if (ScissorDisabled)
	{
		glDisable(GL_SCISSOR_TEST);
	}

	if (IsFirstLevel)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF); 
		glStencilMask(0xFF); // enable writing to the stencil buffer

		m_pCube->Render(m_gliTestProgram, m_pCamera);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // write to areas where value is not equal to 1
		glStencilMask(0x00); //disable writing to stencil buffer

		if (StencilOn)
		{
			m_pScaledCube->Render(m_gliTestProgram, m_pCamera);
		}

		glDisable(GL_STENCIL_TEST);
	}
	else
	{
		//Add fog shader code and make new program
		m_pTransparentCube->Render(m_gliTestProgram, m_pCamera);
		m_pWater->Render(m_gliTestProgram, m_pCamera);
	}
}

void cGameManager::UpdateCurrentLevel(float _deltaTime)
{
	if (IsFirstLevel)
	{
		m_pCube->Update(_deltaTime);
		m_pScaledCube->Update(_deltaTime);
	}
	else
	{
		m_pWater->Update(_deltaTime);
		m_pTransparentCube->Update(_deltaTime);
	}
}

void cGameManager::CheckInput()
{
	//Restart/Switch level
	if (m_pInputManager->KeyState['r'] == InputState::INPUT_DOWN_FIRST && RestartKeyPressed == false)
	{
		//Switch level
		SwitchLevel();
		RestartKeyPressed = true;
	}
	else if (m_pInputManager->KeyState['r'] == InputState::INPUT_UP_FIRST)
	{
		RestartKeyPressed = false;
	}

	//Toggle Stencil
	if (m_pInputManager->KeyState['t'] == InputState::INPUT_DOWN_FIRST && StencilKeyPressed == false)
	{
		ToggleStencilOutline();
		StencilKeyPressed = true;
	}
	else if (m_pInputManager->KeyState['t'] == InputState::INPUT_UP_FIRST)
	{
		StencilKeyPressed = false;
	}

	//Toggle Scissor
	if (m_pInputManager->KeyState['y'] == InputState::INPUT_DOWN_FIRST && ScissorKeyPressed == false)
	{
		ToggleScissor();
		ScissorKeyPressed = true;
	}
	else if (m_pInputManager->KeyState['y'] == InputState::INPUT_UP_FIRST)
	{
		ScissorKeyPressed = false;
	}




}

void cGameManager::ToggleStencilOutline()
{
	StencilOn = !StencilOn;

	if (StencilOn)
	{
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		glClear(GL_STENCIL_BUFFER_BIT);
		glDisable(GL_STENCIL_TEST);
	}
}

void cGameManager::ToggleScissor()
{
	if (glIsEnabled(GL_SCISSOR_TEST))
	{
		//If on turn off
		glDisable(GL_SCISSOR_TEST);
		glScissor(0, 0, 600, 600);
	}
	else
	{
		//Else turn on
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, 600, 500);
	}
}

void cGameManager::CameraMove(float _deltaTime)
{
	//Move Camera
	if (m_pInputManager->KeyState['w'] == InputState::INPUT_DOWN_FIRST)
	{
		//Forward
		m_pCamera->MoveRelative(vec3(0.0f, 0.0f, CamSpeed * _deltaTime));
	}
	if (m_pInputManager->KeyState['s'] == InputState::INPUT_DOWN_FIRST)
	{
		//Back
		m_pCamera->MoveRelative(vec3(0.0f, 0.0f, -CamSpeed * _deltaTime));
	}
	//if (m_pInputManager->KeyState['a'] == InputState::INPUT_DOWN_FIRST)
	//{
	//	//Left
	//	m_pCamera->MoveRelative(vec3(-CamSpeed * _deltaTime, 0.0f, 0.0f));
	//}
	//if (m_pInputManager->KeyState['d'] == InputState::INPUT_DOWN_FIRST)
	//{
	//	//Right	
	//	m_pCamera->MoveRelative(vec3(CamSpeed * _deltaTime, 0.0f, 0.0f));
	//}
}

void cGameManager::MousePick()
{
	//2 Buttons, first makes camera go forward, other backward
	if (m_pInputManager->IsMouseClicked(0))
	{
		if (RayEntityIntersect(m_pTransparentCube))
		{
			//Move cam forward
			cout << "forward" << endl;
		}
		//else if (RayEntityIntersect(m_pCamBackwardButton))
		//{
		//	cout << "Backward" << endl;
		//}
	}
}

bool cGameManager::RayEntityIntersect(cEntity* Object)
{
	bool Intersect = false;
	glm::vec3 rayDirection = vec3(0.0f, 0.0f, 0.0f);

	//screen pos
	glm::vec2 normalizedScreenPos = m_pInputManager->GetMouseCoords();
	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(m_pCamera->GetProjectionMatrix());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(m_pCamera->GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));


	for (int i = 0; i < Object->GetMesh()->GetIndices().size(); i+=3)
	{
		vec3 vec3CamPos = m_pCamera->GetCamPos();
		vec3 vec3Point1 = Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i)).Position;
		vec3 vec3Point2 = Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i+1)).Position;
		vec3 vec3Point3 = Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i+2)).Position;
		vec3 IntersectPoint = vec3(0.0f, 0.0f, 0.0f);
		float something = 0.0f;

		if (glm::intersectRayTriangle(vec3CamPos, rayDirection, vec3Point1, vec3Point2, vec3Point3, IntersectPoint))
		{
			Intersect = true;
		}

		//if (glm::intersectRaySphere(vec3CamPos, glm::normalize(rayDirection), Object->GetTranslate(), 5.0f, something))
		//{
		//	Intersect = true;
		//}

	}
	
	return Intersect;
}

void cGameManager::SwitchLevel()
{
	ResetCurrentLevel();

	//Switch state
	IsFirstLevel = !IsFirstLevel;

}

void cGameManager::ResetCurrentLevel()
{
	//Reset current level objects
	if (IsFirstLevel)
	{
		//Destroy
		m_pCube->~cEntity();
		m_pScaledCube->~cEntity();

		//Re-create
		m_pCube = new cEntity();
		m_pScaledCube = new cEntity();
		m_pWater = new cEntity();

		//Initialise
		m_pCube = InitialiseCube("Resources/Textures/TransparentCube.png");
		m_pScaledCube = InitialiseCube("Resources/Textures/RedOutline.png");
		m_pScaledCube->SetScale(vec3(1.0f, 1.0f, 1.0f), 1.1f);
	}
	else
	{
		//Destroy
		m_pTransparentCube->~cEntity();
		m_pWater->~cEntity();

		//Re-create
		m_pTransparentCube = new cEntity();
		m_pWater = new cEntity();

		//Initialise
		m_pTransparentCube = InitialiseCube("Resources/Textures/TransparentCube.png");
		m_pWater = InitialiseCube("Resources/Textures/Water.png");
		m_pWater->SetScale(vec3(8.0f, 1.0f, 8.0f), 2.0f);
		m_pWater->SetTranslation(m_pWater->GetTranslate() - vec3(0.0f, 1.0f, 0.0f));
	}
}

cEntity* cGameManager::InitialiseCube(string filedir)
{
	cEntity* m_pCube = new cEntity();

	//Sprite parameters
	float CubeFrames = 12.0f;
	int CubeFPS = 12;

	//Size
	float CubeWidth = 12.0f;
	float CubeHeight = 12.0f;
	float fSize = 0.5f;

	//Vertices
	vector <Vertex2D> vecCubeVertices;
	Vertex2D Cubev1;
	Cubev1.Position = vec3(-fSize, fSize, fSize);
	Cubev1.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev1.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Cubev2;
	Cubev2.Position = vec3(-fSize, -fSize, fSize);
	Cubev2.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev2.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev3;
	Cubev3.Position = vec3(fSize, -fSize, fSize);
	Cubev3.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev3.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev4;
	Cubev4.Position = vec3(fSize, fSize, fSize);
	Cubev4.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev4.TexCoords = vec2(1.0f, 0.0f);


	Vertex2D Cubev5;
	Cubev5.Position = vec3(-fSize, fSize, -fSize);
	Cubev5.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev5.TexCoords = vec2(1.0f, 0.0f);

	Vertex2D Cubev6;
	Cubev6.Position = vec3(-fSize, -fSize, -fSize);
	Cubev6.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev6.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev7;
	Cubev7.Position = vec3(fSize, -fSize, -fSize);
	Cubev7.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev7.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev8;
	Cubev8.Position = vec3(fSize, fSize, -fSize);
	Cubev8.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev8.TexCoords = vec2(0.0f, 0.0f);


	Vertex2D Cubev9;
	Cubev9.Position = vec3(-fSize, fSize, -fSize);
	Cubev9.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev9.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Cubev10;
	Cubev10.Position = vec3(-fSize, -fSize, -fSize);
	Cubev10.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev10.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev11;
	Cubev11.Position = vec3(-fSize, -fSize, fSize);
	Cubev11.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev11.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev12;
	Cubev12.Position = vec3(-fSize, fSize, fSize);
	Cubev12.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev12.TexCoords = vec2(1.0f, 0.0f);


	Vertex2D Cubev13;
	Cubev13.Position = vec3(fSize, fSize, fSize);
	Cubev13.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev13.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Cubev14;
	Cubev14.Position = vec3(fSize, -fSize, fSize);
	Cubev14.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev14.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev15;
	Cubev15.Position = vec3(fSize, -fSize, -fSize);
	Cubev15.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev15.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev16;
	Cubev16.Position = vec3(fSize, fSize, -fSize);
	Cubev16.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev16.TexCoords = vec2(1.0f, 0.0f);


	Vertex2D Cubev17;
	Cubev17.Position = vec3(-fSize, fSize, -fSize);
	Cubev17.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev17.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Cubev18;
	Cubev18.Position = vec3(-fSize, fSize, fSize);
	Cubev18.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev18.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev19;
	Cubev19.Position = vec3(fSize, fSize, fSize);
	Cubev19.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev19.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev20;
	Cubev20.Position = vec3(fSize, fSize, -fSize);
	Cubev20.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev20.TexCoords = vec2(1.0f, 0.0f);


	Vertex2D Cubev21;
	Cubev21.Position = vec3(-fSize, -fSize, fSize);
	Cubev21.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev21.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Cubev22;
	Cubev22.Position = vec3(-fSize, -fSize, -fSize);
	Cubev22.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev22.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Cubev23;
	Cubev23.Position = vec3(fSize, -fSize, -fSize);
	Cubev23.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev23.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Cubev24;
	Cubev24.Position = vec3(fSize, -fSize, fSize);
	Cubev24.Color = vec3(0.0f, 1.0f, 0.0f);
	Cubev24.TexCoords = vec2(1.0f, 0.0f);

	vecCubeVertices.push_back(Cubev1);
	vecCubeVertices.push_back(Cubev2);
	vecCubeVertices.push_back(Cubev3);
	vecCubeVertices.push_back(Cubev4);
	vecCubeVertices.push_back(Cubev5);
	vecCubeVertices.push_back(Cubev6);
	vecCubeVertices.push_back(Cubev7);
	vecCubeVertices.push_back(Cubev8);
	vecCubeVertices.push_back(Cubev9);
	vecCubeVertices.push_back(Cubev10);
	vecCubeVertices.push_back(Cubev11);
	vecCubeVertices.push_back(Cubev12);
	vecCubeVertices.push_back(Cubev13);
	vecCubeVertices.push_back(Cubev14);
	vecCubeVertices.push_back(Cubev15);
	vecCubeVertices.push_back(Cubev16);
	vecCubeVertices.push_back(Cubev17);
	vecCubeVertices.push_back(Cubev18);
	vecCubeVertices.push_back(Cubev19);
	vecCubeVertices.push_back(Cubev20);
	vecCubeVertices.push_back(Cubev21);
	vecCubeVertices.push_back(Cubev22);
	vecCubeVertices.push_back(Cubev23);
	vecCubeVertices.push_back(Cubev24);

	//Indices
	vector <GLuint > vecCubeIndices;
	vecCubeIndices.push_back(0);
	vecCubeIndices.push_back(1);
	vecCubeIndices.push_back(2);
	vecCubeIndices.push_back(0);
	vecCubeIndices.push_back(2);
	vecCubeIndices.push_back(3);
	vecCubeIndices.push_back(7);
	vecCubeIndices.push_back(6);
	vecCubeIndices.push_back(5);
	vecCubeIndices.push_back(7);
	vecCubeIndices.push_back(5);
	vecCubeIndices.push_back(4);
	vecCubeIndices.push_back(8);
	vecCubeIndices.push_back(9);
	vecCubeIndices.push_back(10);
	vecCubeIndices.push_back(8);
	vecCubeIndices.push_back(10);
	vecCubeIndices.push_back(11);
	vecCubeIndices.push_back(12);
	vecCubeIndices.push_back(13);
	vecCubeIndices.push_back(14);
	vecCubeIndices.push_back(12);
	vecCubeIndices.push_back(14);
	vecCubeIndices.push_back(15);
	vecCubeIndices.push_back(16);
	vecCubeIndices.push_back(17);
	vecCubeIndices.push_back(18);
	vecCubeIndices.push_back(16);
	vecCubeIndices.push_back(18);
	vecCubeIndices.push_back(19);
	vecCubeIndices.push_back(20);
	vecCubeIndices.push_back(21);
	vecCubeIndices.push_back(22);
	vecCubeIndices.push_back(20);
	vecCubeIndices.push_back(22);
	vecCubeIndices.push_back(23);

	string strCubeImage = "Resources/Textures/NewBall.png";
	int fCubeFrames = 1;

	//Model
	vec3 v3CubeTranslation = vec3(0.0f, -0.0f, 0.0f);
	vec3 v3CubeRotation = vec3(0.0f, 0.0f, 1.0f);
	float fCubeRotAngle = 0.0f;
	vec3 v3CubeScale = vec3(1.0f, 1.0f, 1.0f);
	float fCubeScaleFactor = 1.0f;

	m_pCube->Intitialise(vecCubeVertices, vecCubeIndices, fCubeFrames, filedir,
		v3CubeTranslation, v3CubeRotation, fCubeRotAngle,
		v3CubeScale, fCubeScaleFactor, CubeFPS, CubeWidth, CubeHeight);

	return m_pCube;
}

cEntity* cGameManager::InitialiseQuad(string filedir)
{
	cEntity* Quad = new cEntity();

	int QuadFPS = 1;
	vector <Vertex2D> vecQuadRectVertices;

	float QuadWidth = 10.0f;
	float QuadHeight = 10.0f;

	Vertex2D Quadv1;
	Quadv1.Position = vec3(-QuadWidth, 0.0f, QuadHeight);
	Quadv1.Color = vec3(0.0f, 1.0f, 0.0f);
	Quadv1.TexCoords = vec2(0.0f, 0.0f);

	Vertex2D Quadv2;
	Quadv2.Position = vec3(-QuadHeight, 0.0f, -QuadWidth);
	Quadv2.Color = vec3(1.0f, 0.0f, 0.0f);
	Quadv2.TexCoords = vec2(0.0f, 1.0f);

	Vertex2D Quadv3;
	Quadv3.Position = vec3(QuadHeight, 0.0f, -QuadHeight);
	Quadv3.Color = vec3(1.0f, 1.0f, 0.0f);
	Quadv3.TexCoords = vec2(1.0f, 1.0f);

	Vertex2D Quadv4;
	Quadv4.Position = vec3(QuadHeight, 0.0f, QuadWidth);
	Quadv4.Color = vec3(0.0f, 0.0f, 1.0f);
	Quadv4.TexCoords = vec2(1.0f, 0.0f);

	vecQuadRectVertices.push_back(Quadv1);
	vecQuadRectVertices.push_back(Quadv2);
	vecQuadRectVertices.push_back(Quadv3);
	vecQuadRectVertices.push_back(Quadv4);

	vector <GLuint > vecQuadRectIndices;

	vecQuadRectIndices.push_back(0);
	vecQuadRectIndices.push_back(1);
	vecQuadRectIndices.push_back(2);
	vecQuadRectIndices.push_back(0);
	vecQuadRectIndices.push_back(2);
	vecQuadRectIndices.push_back(3);

	string strQuadImage = filedir;
	vec3 v3QuadRotation = vec3(0.0f, 0.0f, 1.0f);
	int fQuadFrames = 12;

	vec3 v3QuadTranslation = vec3(0.0f, 0.0f, 0.0f);
	float fQuadRotAngle = 0.0f;
	vec3 v3QuadScale = vec3(1.0f, 1.0f, 1.0f);
	float fQuadScaleFactor = 1.0f;

	Quad->Intitialise(vecQuadRectVertices, vecQuadRectIndices, fQuadFrames, strQuadImage,
		v3QuadTranslation, v3QuadRotation, fQuadRotAngle,
		v3QuadScale, fQuadScaleFactor, QuadFPS, QuadWidth, QuadHeight);

	return Quad;
}


