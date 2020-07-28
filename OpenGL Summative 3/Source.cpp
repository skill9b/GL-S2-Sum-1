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

int main(int argc, char** argv)
{
	srand((unsigned)(time(0)));
	//Setup and create at glut controlled window
	glutInit(&argc, argv);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("3D Shooter");

	//Enable settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Sets up all gl function callbacks
	if (glewInit() != GLEW_OK)
	{
		//If glew setup fails then application will not run graphics properly
		cout << "Glew Initialization Failed. Aborting Application." << endl;
		system("pause");
	}

	//Sets the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black


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

