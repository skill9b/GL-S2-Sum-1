#pragma once
#include "Global.h"
#include "cEntity.h"
#include "cText.h"
#include "cInput.h"

class cInput;

class cMenu
{
	
private:
	//Member Variables
	cText* m_pTitle;
	cText* m_pPlayButton;
	cText* m_pQuitButton;
	cInput* m_pInputManager;
	int m_iIndex;
	State m_SelectedLevel;
	bool m_bKeyPressed;

public:
	//Member Functions
	cMenu(); //Constructor
	~cMenu(); //Destructor
	bool Initialise();
	void Update(cInput* _pInputManager);
	void Render();
	void MoveIndexDown();
	void MoveIndexUp();
	State Select();
	void ProcessInput(cInput* _pInputManager);
	void ChangeColor();

	State GetSelectedLevel();
	cInput* GetInputManager();
	void ResetSelectedLevel();
};

