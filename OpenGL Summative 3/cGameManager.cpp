#include "cGameManager.h"
#include <gtx/intersect.hpp>

cGameManager::cGameManager()
{
	m_CurrentState = State::Menu;
	m_pInputManager = new cInput();
	m_pMainMenu = new cMenu();
	m_pLevelOne = new cLevel();
	m_pCamera = new cCamera();
	m_pCubeMap = 0;


	m_pCube = new cEntity();
	m_pScaledCube = new cEntity();
	m_pWater = new cEntity();


	m_gliBasicProgram = 0;
	m_gliTestProgram = 0;
	m_fScreenWidth = SCR_WIDTH;
	m_fScreenHeight = SCR_HEIGHT;

	CamSpeed = 25.0f;
	CubeSpeed = 25.0f;
	IsFirstLevel = false;
	StencilOn = true;
	StencilKeyPressed = false;
	ScissorKeyPressed = false;
	RestartKeyPressed = false;
	ScissorDisabled = false;
	WireFrameDisabled = true;
	BackCullDisabled = false;
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

	m_pCamera->Initialise(SCR_WIDTH, SCR_HEIGHT, 10000.0f, 0.1f);
	m_pCubeMap = new cCubeMap(m_pCamera);

	m_pCube = InitialiseCube("Resources/Textures/YellowCube.png");
	m_pCube->SetTranslation(m_pCube->GetTranslate() + vec3 (0.0f, 3.0f, 0.0f));
	m_pScaledCube = InitialiseCube("Resources/Textures/RedOutline.png");
	m_pScaledCube->SetScale(vec3(1.0f, 1.0f, 1.0f), 1.2f);
	m_pScaledCube->SetTranslation(m_pCube->GetTranslate());

	m_pTransparentCube = InitialiseCube("Resources/Textures/TransparentCube.png");
	m_pWater = InitialiseCube("Resources/Textures/Water.png");
	m_pWater->SetTranslation(m_pTransparentCube->GetTranslate() + vec3(0.0f, 1.0f, 0.0f));

	m_pCamForwardButton = InitialiseCube("Resources/Textures/CamForward.png");
	m_pCamForwardButton->SetTranslation(vec3(3.5f, 0.0f, 0.0f));

	m_pCamBackwardButton = InitialiseCube("Resources/Textures/CamBackward.png");
	m_pCamBackwardButton->SetTranslation(vec3(-3.5f, 0.0f, 0.0f));

	m_pCubeForwardButton = InitialiseCube("Resources/Textures/CubeForward.png");
	m_pCubeForwardButton->SetTranslation(vec3(1.5f, 0.0f, 0.0f));

	m_pCubeBackwardButton = InitialiseCube("Resources/Textures/CubeBackward.png");
	m_pCubeBackwardButton->SetTranslation(vec3(-1.5f, 0.0f, 0.0f));

	//Program
	m_gliBasicProgram = ShaderLoader::CreateProgram("Resources/Shaders/Basic.vs",
		"Resources/Shaders/Basic.fs");

	m_gliTestProgram = ShaderLoader::CreateProgram("Resources/Shaders/Fog.vs",
		"Resources/Shaders/Fog.fs");

	Update(_deltaTime);
}
void cGameManager::ResetCurrentLevel(float _deltaTime)
{
	m_pCamera = new cCamera();
	m_pCamera->Initialise(SCR_WIDTH, SCR_HEIGHT, 10000.0f, 0.1f);

	m_pTransparentCube = InitialiseCube("Resources/Textures/TransparentCube.png");
	m_pTransparentCube->SetTranslation(vec3(0.0f, 0.0f, 0.0f));

	m_pCamForwardButton->SetTranslation(vec3(3.5f, 0.0f, 0.0f));
	m_pCamBackwardButton->SetTranslation(vec3(-3.5f, 0.0f, 0.0f));

	glStencilMask(0xFF); //enable writing to stencil buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00); //disable writing to stencil buffer
}
void cGameManager::Update(float _deltaTime)
{

	//Update persistant objects	
	CameraMove(_deltaTime);
	MousePick(_deltaTime);
	m_pCamera->Update(_deltaTime);
	m_pCubeMap->Update();
	CheckInput(_deltaTime);
	UpdateCurrentLevel(_deltaTime);
	
	glutPostRedisplay();
}

void cGameManager::Render()
{
	glStencilMask(0xFF); //enable writing to stencil buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00); //disable writing to stencil buffer

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
	else
	{
		//Clear buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, 600, 500);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

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

	m_pCamForwardButton->Render(m_gliTestProgram, m_pCamera);
	m_pCamBackwardButton->Render(m_gliTestProgram, m_pCamera);

	m_pCubeForwardButton->Render(m_gliTestProgram, m_pCamera);
	m_pCubeBackwardButton->Render(m_gliTestProgram, m_pCamera);
		
	m_pTransparentCube->Render(m_gliTestProgram, m_pCamera);
	m_pWater->Render(m_gliTestProgram, m_pCamera);
}

void cGameManager::UpdateCurrentLevel(float _deltaTime)
{
	m_pCube->Update(_deltaTime);
	m_pScaledCube->Update(_deltaTime);

	m_pCamForwardButton->Update(_deltaTime);
	m_pCamBackwardButton->Update(_deltaTime);

	m_pCubeForwardButton->Update(_deltaTime);
	m_pCubeBackwardButton->Update(_deltaTime);

	m_pWater->Update(_deltaTime);
	m_pTransparentCube->Update(_deltaTime);
}

void cGameManager::CheckInput(float _deltaTime)
{
	//Restart/Switch level
	if (m_pInputManager->KeyState['r'] == InputState::INPUT_DOWN_FIRST && RestartKeyPressed == false)
	{
		//Switch level
		SwitchLevel(_deltaTime);
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

	//Toggle Wireframe
	if (m_pInputManager->KeyState['u'] == InputState::INPUT_DOWN_FIRST && WireKeyPressed == false)
	{
		ToggleWireFrame();
		WireKeyPressed = true;
	}
	else if (m_pInputManager->KeyState['u'] == InputState::INPUT_UP_FIRST)
	{
		WireKeyPressed = false;
	}

	//Toggle Backface culling
	if (m_pInputManager->KeyState['i'] == InputState::INPUT_DOWN_FIRST && CullKeyPressed == false)
	{
		ToggleCull();
		CullKeyPressed = true;
	}
	else if (m_pInputManager->KeyState['i'] == InputState::INPUT_UP_FIRST)
	{
		CullKeyPressed = false;
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
	ScissorDisabled = !ScissorDisabled;
}

void cGameManager::CameraMove(float _deltaTime)
{
	//Move Camera
	if (m_pInputManager->KeyState['a'] == InputState::INPUT_DOWN_FIRST)
	{
		//Left
		m_pCamera->MoveRelative(vec3(CamSpeed * _deltaTime, 0.0f, 0.0f));
	}
	else if (m_pInputManager->KeyState['d'] == InputState::INPUT_DOWN_FIRST)
	{
		//Right
		m_pCamera->MoveRelative(vec3(-CamSpeed * _deltaTime, 0.0f, 0.0f));
	}

	if (m_pInputManager->KeyState['w'] == InputState::INPUT_DOWN_FIRST)
	{
		//Forward
		m_pCamera->MoveRelative(vec3(0.0f, 0.0f, CamSpeed * _deltaTime));
	}
	else if (m_pInputManager->KeyState['s'] == InputState::INPUT_DOWN_FIRST)
	{
		//Back
		m_pCamera->MoveRelative(vec3(0.0f, 0.0f, -CamSpeed * _deltaTime));

	}
}


void cGameManager::MousePick(float _deltaTime)
{
	if (m_pInputManager->IsMouseClicked(0))
	{
		//Move cam
		if (RayEntityIntersect(m_pCamForwardButton))
		{
			//Forward
			m_pCamera->MoveRelative(vec3(0.0f, 0.0f, CamSpeed * _deltaTime));
		}
		if (RayEntityIntersect(m_pCamBackwardButton))
		{
			//Back
			m_pCamera->MoveRelative(vec3(0.0f, 0.0f, -CamSpeed * _deltaTime));
		}

		//Move cube
		if (RayEntityIntersect(m_pCubeForwardButton))
		{
			//Forward
			m_pTransparentCube->SetTranslation(m_pTransparentCube->GetTranslate() + vec3(0.0f, 0.0f, CubeSpeed * _deltaTime));
		}
		if (RayEntityIntersect(m_pCubeBackwardButton))
		{
			//Back
			m_pTransparentCube->SetTranslation(m_pTransparentCube->GetTranslate() + vec3(0.0f, 0.0f, -CubeSpeed * _deltaTime));
		}
	}
}

bool cGameManager::RayEntityIntersect(cEntity* Object)
{
	bool Intersect = false;
	glm::vec3 vec3RayDirection = vec3(0.0f, 0.0f, 0.0f);

	//screen pos
	glm::vec2 vec2NormalizedScreenPos = m_pInputManager->GetMouseCoords();
	//screenpos to Proj Space
	glm::vec4 vec4ClipCoords = glm::vec4(vec2NormalizedScreenPos.x, vec2NormalizedScreenPos.y, -1.0f, 1.0f);

	//Proj Space to eye space
	glm::mat4 mat4InvProjMat = glm::inverse(m_pCamera->GetProjectionMatrix());
	glm::vec4 vec4EyeCoords = mat4InvProjMat * vec4ClipCoords;
	vec4EyeCoords = glm::vec4(vec4EyeCoords.x, vec4EyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 m4InvViewMat = glm::inverse(m_pCamera->GetViewMatrix());
	glm::vec4 vec4RayWorld = m4InvViewMat * vec4EyeCoords;
	vec3RayDirection = glm::normalize(glm::vec3(vec4RayWorld));

	vec3 vec3CamPos = m_pCamera->GetCamPos();

	for (int i = 0; i < Object->GetMesh()->GetIndices().size(); i+=3)
	{
		vec4 vec4Point1 = Object->GetModel() * vec4(Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i)).Position, 1.0f);
		vec4 vec4Point2 = Object->GetModel() * vec4(Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i + 1)).Position, 1.0f);
		vec4 vec4Point3 = Object->GetModel() * vec4(Object->GetMesh()->GetVertices().at(Object->GetMesh()->GetIndices().at(i + 2)).Position, 1.0f);

		vec3 IntersectPoint = vec3(0.0f, 0.0f, 0.0f);

		if (glm::intersectRayTriangle(vec3CamPos, vec3RayDirection, vec3(vec4Point1), vec3(vec4Point2), vec3(vec4Point3), IntersectPoint))
		{
			Intersect = true;
		}
	}
	
	return Intersect;
}

void cGameManager::ToggleCull()
{
	BackCullDisabled = !BackCullDisabled;
	BackCullDisabled ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
}

void cGameManager::ToggleWireFrame()
{
	WireFrameDisabled = !WireFrameDisabled;
	WireFrameDisabled ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void cGameManager::SwitchLevel(float _deltaTime)
{
	ResetCurrentLevel(_deltaTime);

	//Switch state
	IsFirstLevel = !IsFirstLevel;
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

	GLuint indices[] = {
		0, 1, 2, // Front
		0, 2, 3, // Front

		7, 6, 5, // Back
		7, 5, 4, // Back

		8, 9, 10, // Left
		8, 10, 11, // Left

		12, 13, 14, // Right
		12, 14, 15, // Right

		16, 17, 18, // Top
		16, 18, 19, // Top

		20, 21, 22, // Bottom
		20, 22, 23, // Bottom
	};

	for (GLuint i = 0; i < 36; i++)
	{
		vecCubeIndices.push_back(indices[i]);
	}

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

	float QuadWidth = 1.0f;
	float QuadHeight = 1.0f;

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


