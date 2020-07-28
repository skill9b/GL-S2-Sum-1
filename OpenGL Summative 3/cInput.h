#pragma once
#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Global.h"
using namespace std;

enum InputState
{
	INPUT_UP,
	INPUT_DOWN,
	INPUT_UP_FIRST,
	INPUT_DOWN_FIRST,
};

class cMenu;

class cInput
{
private:
	
	glm::vec2 m_v2MouseCoords = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_v2ClickCoords = glm::vec2(0.0f, 0.0f);

	bool m_bFirstClick = false;
public:
	InputState KeyState[255] = {};
	InputState MouseState[3] = {};
	
	
	glm::vec2 GetMouseCoords();
	glm::vec2 GetClickedCoords();
	bool IsMouseClicked(int _button);

	void KeyboardDown(unsigned char _key, int x, int y);
	void KeyboardUp(unsigned char _key, int x, int y);

	void MouseClick(int button, int state, int x, int y);
	void MousePassiveMove(int x, int y);
	void MouseMove(int x, int y);
};

