#include "cInput.h"

glm::vec2 cInput::GetMouseCoords()
{
	return m_v2MouseCoords;
}

glm::vec2 cInput::GetClickedCoords()
{
	return m_v2ClickCoords;
}

bool cInput::IsMouseClicked(int _button)
{
	//Only register first click
	//if ((MouseState[_button] == InputState::INPUT_DOWN) && (m_bFirstClick))
	if ((MouseState[_button] == InputState::INPUT_DOWN))
	{
		m_bFirstClick = false;
		return true;
	}
	else if ((MouseState[_button] == InputState::INPUT_UP))
	{
		m_bFirstClick = true;
	}
	return false;
}

void cInput::KeyboardDown(unsigned char _key, int x, int y)
{
	if ((KeyState[_key] == InputState::INPUT_UP) || (KeyState[_key] == InputState::INPUT_UP_FIRST))
	{
		KeyState[_key] = InputState::INPUT_DOWN_FIRST;
	}
}

void cInput::KeyboardUp(unsigned char _key, int x, int y)
{
	if ((KeyState[_key] == InputState::INPUT_DOWN) || (KeyState[_key] == InputState::INPUT_DOWN_FIRST))
	{
		KeyState[_key] = InputState::INPUT_UP_FIRST;
	}
}

void cInput::MouseClick(int button, int state, int x, int y)
{
	if (button >= 3)
	{
		return;
	}

	MouseState[button] = (state == GLUT_DOWN) ? InputState::INPUT_DOWN : InputState::INPUT_UP;


	m_v2ClickCoords.x = (float)x - (float)(SCR_WIDTH * 0.5f);
	m_v2ClickCoords.y = -((float)y - (float)(SCR_HEIGHT * 0.5f));
}

void cInput::MousePassiveMove(int x, int y)
{
	//Offset mouse pos to start from centre and not top left
	//m_v2MouseCoords.x = (float)x - (float)(SCR_WIDTH * 0.5f);
	//m_v2MouseCoords.y = -((float)y - (float)(SCR_HEIGHT * 0.5f));

	m_v2MouseCoords.x = (2.0f * x) / (float)SCR_WIDTH - 1.0f;
	m_v2MouseCoords.y = 1.0f - (2.0f * y) / (float)SCR_HEIGHT;
}

void cInput::MouseMove(int x, int y)
{
	MousePassiveMove(x, y);
}





