#include "cMenu.h"

cMenu::cMenu()
{
	m_pTitle = 0;
	m_pPlayButton = 0;
	m_pQuitButton = 0;

	m_pInputManager = new cInput();
	m_iIndex = 0;
	m_SelectedLevel = State::Menu;
	m_bKeyPressed = false;
}

cMenu::~cMenu()
{
	delete m_pTitle;
	delete m_pPlayButton;
	delete m_pInputManager;
	delete m_pQuitButton;

	m_pTitle = 0;
	m_pInputManager = 0;
	m_pPlayButton = 0;
	m_pQuitButton = 0;
}

bool cMenu::Initialise()
{
	m_pTitle = new cText("Welcome To Pug Invasion", "Resources/Fonts/arial.ttf",
						 vec2(-210.0f, 250.0f), vec3(1.0f, 0.0f, 0.0f), 0.75f);

	m_pPlayButton = new cText("Play", "Resources/Fonts/arial.ttf",
						 vec2(-30.0f, 100.0f), vec3(1.0f, 1.0f, 1.0f), 0.5f);

	m_pQuitButton = new cText("Quit", "Resources/Fonts/arial.ttf",
							vec2(-30.0f, 75.0f), vec3(1.0f, 1.0f, 1.0f), 0.5f);

	return true;
}

void cMenu::Update(cInput* _pInputManager)
{
	ChangeColor();
	ProcessInput(_pInputManager);
}

void cMenu::Render()
{
	m_pTitle->Render();
	m_pPlayButton->Render();
	m_pQuitButton->Render();

}

void cMenu::MoveIndexDown()
{
	if (m_iIndex >= 1)
	{
		m_iIndex = 0;
	}
	else
	{
		m_iIndex++;
	}
}

void cMenu::MoveIndexUp()
{
	if (m_iIndex <= 0)
	{
		m_iIndex = 1;
	}
	else
	{
		m_iIndex--;
	}
}

State cMenu::Select()
{
	switch (m_iIndex)
	{
	case 0:
	{
		//Play
		return State::InGame;
		break;
	}
	case 1:
	{
		//Quit
		glutDestroyWindow(1);
		break;
	}
	default:
	{
		return State::Menu;
		break;
	}
	}

}

void cMenu::ProcessInput(cInput* _pInputManager)
{
	//Move up
	if ( (_pInputManager->KeyState['w'] == INPUT_DOWN_FIRST) || (_pInputManager->KeyState['W'] == INPUT_DOWN_FIRST) )
	{
		if (m_bKeyPressed == false)
		{
			MoveIndexUp();
			m_bKeyPressed = true;
		}
	}
	//Move down
	else if ( (_pInputManager->KeyState['s'] == INPUT_DOWN_FIRST)|| (_pInputManager->KeyState['S'] == INPUT_DOWN_FIRST) )
	{
		if (m_bKeyPressed == false)
		{
			MoveIndexDown();
			m_bKeyPressed = true;
		}
	}
	//Select
	else if (_pInputManager->KeyState[' '] == INPUT_DOWN_FIRST)
	{
		m_SelectedLevel = Select();
		_pInputManager->KeyState[' '] = INPUT_UP;
	}
	//Reset
	else
	{
		m_bKeyPressed = false;
	}
}

void cMenu::ChangeColor()
{
	//Set all to white
	m_pPlayButton->SetColor(vec3(1.0f, 1.0f, 1.0f));
	m_pQuitButton->SetColor(vec3(1.0f, 1.0f, 1.0f));

	//Set selected to red
	switch (m_iIndex)
	{
	case 0:
	{
		m_pPlayButton->SetColor(vec3(1.0f, 0.0f, 0.0f));
		break;
	}
	case 1:
	{
		m_pQuitButton->SetColor(vec3(1.0f, 0.0f, 0.0f));
		break;
	}
	}
}

State cMenu::GetSelectedLevel()
{
	return m_SelectedLevel;
}

cInput* cMenu::GetInputManager()
{
	return m_pInputManager;
}

void cMenu::ResetSelectedLevel()
{
	//Restart menu
	ChangeColor();
	m_iIndex = 0;
	m_SelectedLevel = State::Menu;
}
