#include "Global.h"
#include "cGameManager.h"
#include "cEntity.h"
#include <chrono>
#include <random>

cGameManager GameManager;
float fOldTimeSinceStart = 0;
float fTimeSinceStart;
float fDeltaTime;

void Render();
void Update();
void KeyboardUpCallback(unsigned char _key, int x, int y);
void KeyboardDownCallback(unsigned char _key, int x, int y);
void MousePassiveCallback(int x, int y);
void MouseMoveCallback(int x, int y);
void MouseClickCallback(int button, int state, int x, int y);

int main(int argc, char** argv)
{
	srand((unsigned)(time(0)));
	//Setup and create at glut controlled window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("3D Showcase");

	//Enable settings

	//Black bars on top and bottom
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 50, 600, 500);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00); //disable writing to stencil mask

	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//Sets up all gl function callbacks
	if (glewInit() != GLEW_OK)
	{
		//If glew setup fails then application will not run graphics properly
		cout << "Glew Initialization Failed. Aborting Application." << endl;
		system("pause");
	}

	//Sets the clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //Grey


	GameManager.Initialise(0.0f);

	//Calculate delta time after intitialise
	fTimeSinceStart = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
	fDeltaTime = fTimeSinceStart - fOldTimeSinceStart;
	fOldTimeSinceStart = fTimeSinceStart;

	//Register Callbacks
	glutDisplayFunc(Render);
	glutIdleFunc(Update);

	glutKeyboardFunc(KeyboardDownCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);
	glutMouseFunc(MouseClickCallback);
	glutMotionFunc(MouseMoveCallback);
	glutPassiveMotionFunc(MousePassiveCallback);

	glutMainLoop(); //Ensure this is the last glut line called

	return(0);
}

void Render()
{
	GameManager.Render();
}

void Update()
{
	//Calculate delta time
	fTimeSinceStart = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
	fDeltaTime = fTimeSinceStart - fOldTimeSinceStart;
	fOldTimeSinceStart = fTimeSinceStart;
	
	GameManager.Update(fDeltaTime);
}

void KeyboardDownCallback(unsigned char _key, int x, int y)
{
	GameManager.GetInputManager()->KeyboardDown(_key, x, y);
	
}


void KeyboardUpCallback(unsigned char _key, int x, int y)
{
	GameManager.GetInputManager()->KeyboardUp(_key, x, y);
}

void MousePassiveCallback(int x, int y)
{
	GameManager.GetInputManager()->MousePassiveMove(x, y);
}

void MouseClickCallback(int button, int state, int x, int y) 
{
	GameManager.GetInputManager()->MouseClick(button, state, x, y);
}

void MouseMoveCallback(int x, int y)
{
	GameManager.GetInputManager()->MouseMove(x, y);
}