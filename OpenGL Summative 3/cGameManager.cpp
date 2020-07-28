#include "cGameManager.h"

cGameManager::cGameManager()
{
	m_CurrentState = State::Menu;
	m_pInputManager = new cInput();
	m_pMainMenu = new cMenu();
	m_pLevelOne = new cLevel();
	m_pCamera = new cCamera();
	m_pBackground = new cEntity();
	m_pCubeMap = 0;


	m_gliReflectionProgram = 0;
	m_fScreenWidth = SCR_WIDTH;
	m_fScreenHeight = SCR_HEIGHT;
}

cGameManager::~cGameManager()
{
	delete m_pLevelOne;
	delete m_pMainMenu;
	delete m_pCamera;
	delete m_pInputManager;

	m_pLevelOne = 0;
	m_pMainMenu = 0;
	m_pCamera = 0;
	m_pInputManager = 0;
}

void cGameManager::Initialise(float _deltaTime)
{
	//Initilise member variables
	m_pLevelOne->Initialise(m_pCamera);
	m_pMainMenu->Initialise();
	m_pCamera->Initialise(SCR_WIDTH, SCR_HEIGHT, 10000.0f, 0.1f);
	m_pCubeMap = new cCubeMap(m_pCamera);

	//Program
	m_gliReflectionProgram = ShaderLoader::CreateProgram("Resources/Shaders/Reflection.vs",
		"Resources/Shaders/Reflection.fs");

	Update(_deltaTime);
}

void cGameManager::Update(float _deltaTime)
{
	//Update persistant objects	
	m_pCamera->Update(_deltaTime);
	m_pCubeMap->Update();


	switch (m_CurrentState)
	{
	case State::Menu:	//If in menu
		{
			m_pMainMenu->Update(m_pInputManager);
			m_CurrentState = m_pMainMenu->GetSelectedLevel();
		}
		break;

	case State::InGame:  //If in game
		{
			m_pLevelOne->Update(_deltaTime, m_pInputManager, m_pCamera);
			if (m_pLevelOne->GetInGameState())
			{
				m_CurrentState = State::Restart;
			}
		}
		break;

	case State::Restart:
		{
			//Reset level and set state to menu
			m_pMainMenu->ResetSelectedLevel();
			delete m_pLevelOne;
			m_pLevelOne = 0;
			m_pLevelOne = new cLevel();
			m_pLevelOne->RestartGame();
			m_pLevelOne->Initialise(m_pCamera);
			m_CurrentState = State::Menu;
		}
		break;
	default:
		break;
	}

	

	glutPostRedisplay();
}

void cGameManager::Render()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Render persistant objects
	m_pCubeMap->Render();

	switch (m_CurrentState)
	{
	case State::Menu: //Render menu
	{
		m_pMainMenu->Render();
	}
	break;

	case State::InGame: //Render game
	{
		m_pLevelOne->Render(m_pCamera, m_gliReflectionProgram, m_pCubeMap);
	}
	break;

	case State::Restart: //Render end game
	{
		m_pLevelOne->Render(m_pCamera, m_gliReflectionProgram, m_pCubeMap);
	}
	break;

	default:
		break;
	}

	glutSwapBuffers();
}

cInput* cGameManager::GetInputManager()
{
	return m_pInputManager;
}
