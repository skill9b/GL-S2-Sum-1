#include "cLevel.h"

cLevel::cLevel()
{
	m_fFireTimer = 0.0f;
	m_pLoading = 0;
	m_pInstructions = 0;
	m_pScore = 0;
	m_pLives = 0;
	m_iLives = 3;
	m_bDisplayResults = false;
	m_bEndGame = false;
	m_pTank = 0;
	m_pSphereMesh = 0;
	m_pAudioManager = new cAudio();
	m_pInputManager = new cInput();
	m_iSpawnCount = 0;
	m_fLastSpawn = 0.0f;
	m_SpawnTimer = 0.0f;
	m_v3EnemyVelocity = vec3(0.0f, 0.0f, 0.0f);
	m_iScore = 0;
}

cLevel::~cLevel()
{
	m_iScore = 0;
	delete m_pAudioManager;
	delete m_pInputManager;
	delete m_pSphereMesh;
	delete m_pScore;
	delete m_pLives;
	delete m_pInstructions;
	delete m_pLoading;
	delete m_pTank;

	m_pAudioManager = 0;
	m_pInputManager = 0;
	m_pSphereMesh = 0;
	m_pScore = 0;
	m_pLives = 0;
	m_pInstructions = 0;
	m_pLoading = 0;
	m_pTank = 0;
}

void cLevel::Initialise(cCamera* _camera)
{
	//Create models
	m_pTank = new cPlayer("Resources/Models/Tank/Tank.obj", _camera);

	//Mesh
	m_pSphereMesh = new Sphere();

	//Managers
	m_pAudioManager = new cAudio();
	m_pAudioManager->Initialise();
	m_pAudioManager->PlaySound(m_pAudioManager->m_pBGM);

	m_pInputManager = new cInput();


	//Text
	m_pScore = new cText("Score: ", "Resources/Fonts/arial.ttf",
		vec2(-55.0f, 275.0f), vec3(1.0f, 0.0f, 0.0f), 0.5f);

	m_pLives = new cText("Lives: ", "Resources/Fonts/arial.ttf",
		vec2(-53.0f, 255.0f), vec3(1.0f, 0.0f, 0.0f), 0.5f);

	m_pInstructions = new cText("", "Resources/Fonts/arial.ttf",
		vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.5f);

	m_pLoading = new cText("", "Resources/Fonts/arial.ttf",
		vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 0.5f);


	m_iLives = 3;
	m_iScore = 0;
	m_iSpawnCount = 1;
	m_fLastSpawn = 0.0f;
	m_SpawnTimer = 5.0f;
	m_v3EnemyVelocity = vec3(0.0f, 0.0f, 3.5f);
	m_fFireTimer = 0.5f;
	m_bDisplayResults = false;
	m_bEndGame = false;

}

void cLevel::Update(float _deltaTime, cInput* _inputManager, cCamera* _camera)
{
	UpdateLives();
	if (m_bDisplayResults == false)
	{
		//Pre-frame checks
		BulletCollisions();
		CheckShoot(_deltaTime);
		SpawnEnemies(_deltaTime, _camera);

		//Update Text
		m_pScore->SetText("Score: " + to_string(m_iScore));
		m_pLives->SetText("Lives: " + to_string(m_iLives));

		//Input
		m_pTank->Rotate(_deltaTime, _inputManager);
		m_pTank->Shoot(_deltaTime, _inputManager, m_vecBullets);

		//Move
		MoveBullets(_deltaTime);
		DeleteBullets();
		MoveEnemies(_deltaTime);

		//Update
		m_pTank->Update(_deltaTime);
		UpdateBullets(_deltaTime);
		UpdateEnemies(_deltaTime);
	}
	else
	{
		//Input
		RestartInput(_inputManager);

		//Text
		m_pScore->SetPosition(vec2(-55.0f, 0.0f));
		m_pInstructions->SetText("You lost, press space to go to menu");
		m_pInstructions->SetPosition(vec2(-200.0f, -20.0f));
		m_pLoading->SetText("Please wait a moment for the to game reload");
		m_pLoading->SetPosition(vec2(-240.0f, -40.0f));
	}

}

void cLevel::Render(cCamera* _camera, GLuint _reflectionProgram, cCubeMap* _cubeMap)
{
	m_pScore->Render();

	//If in game
	if (m_bDisplayResults == false)
	{
		m_pLives->Render();
		RenderBullets(_camera, _reflectionProgram, _cubeMap);
		RenderEnemies();
		m_pTank->Render();
	}
	//Results
	else
	{
		m_pInstructions->Render();
		if (m_bEndGame)
		{
			m_pLoading->Render();
		}
	}
}

bool cLevel::GetInGameState()
{
	return m_bEndGame;
}

void cLevel::RestartInput(cInput* _inputManager)
{
	if (_inputManager->KeyState[' '] == InputState::INPUT_DOWN_FIRST)
	{
		//Go to menu
		m_bEndGame = true;
	}
}

void cLevel::RestartGame()
{
	//Delete pointers
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		delete (*i);
	}

	for (auto i = m_vecEnemies.begin(); i < m_vecEnemies.end(); i++)
	{
		delete (*i);
	}

	//Clear
	m_vecBullets.clear();
	m_vecEnemies.clear();
}

void cLevel::DeleteBullets()
{
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		//Check range
		if (((*i)->GetTranslate().x >= 100.0f) || ((*i)->GetTranslate().z >= 100.0f) ||
			((*i)->GetTranslate().x <= -100.0f) || ((*i)->GetTranslate().z <= -100.0f))
		{
			//Delete 
			delete (*i);

			//Erase
			m_vecBullets.erase(i);
			break;
		}
	}

}

void cLevel::MoveBullets(float _deltaTime)
{
	//Move all active bullets
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		(*i)->Move(_deltaTime);
	}
}

void cLevel::BulletCollisions()
{
	//For every bullet
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		//Check collisions with every enemy
		for (auto j = m_vecEnemies.begin(); j < m_vecEnemies.end(); j++)
		{
			//Check in collision range
			if ((float)glm::distance((*i)->GetTranslate(), (*j)->GetTranslate()) <= (*i)->m_fRadius)
			{
				//Increase score
				m_iScore++;

				//Play sound
				m_pAudioManager->PlaySound(m_pAudioManager->m_pfxBallCollides);

				//Delete and erase collided objects
				delete (*i);
				m_vecBullets.erase(i);
				delete (*j);
				m_vecEnemies.erase(j);
				return;
			}
		}
	}
}

void cLevel::RenderBullets(cCamera* _camera, GLuint _reflectionProgram, cCubeMap* _cubeMap)
{
	//Render all bullets
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		(*i)->Render(_reflectionProgram, _camera, m_pSphereMesh->GetVAO(), m_pSphereMesh->GetIndice(), _cubeMap);
	}
}

void cLevel::UpdateBullets(float _deltaTime)
{
	//Update position of all bullets
	for (auto i = m_vecBullets.begin(); i < m_vecBullets.end(); i++)
	{
		(*i)->Update(_deltaTime);
	}
}

void cLevel::CheckShoot(float _deltaTime)
{
	//Limit fire timer
	if (m_pTank->IsFired() == true)
	{
		m_fFireTimer -= _deltaTime;

		//Reset if gone past timer
		if (m_fFireTimer <= 0.0f)
		{
			m_pTank->ResetFire();
			m_fFireTimer = 0.5f;
		}
	}
}

void cLevel::MoveEnemies(float _deltaTime)
{
	//Move all active enemies
	for (auto i = m_vecEnemies.begin(); i < m_vecEnemies.end(); i++)
	{
		(*i)->Move(_deltaTime);
	}
}

void cLevel::UpdateEnemies(float _deltaTime)
{
	//Update all active enemy positions
	for (auto i = m_vecEnemies.begin(); i < m_vecEnemies.end(); i++)
	{
		(*i)->Update(_deltaTime);
	}
}

void cLevel::RenderEnemies()
{
	//Render all enemies
	for (auto i = m_vecEnemies.begin(); i < m_vecEnemies.end(); i++)
	{
		(*i)->Render();
	}
}

void cLevel::SpawnEnemies(float _deltaTime, cCamera* camera)
{
	//store in variable and reduce over time
	m_fLastSpawn += _deltaTime;

	//When less than timer spawn
	if (m_fLastSpawn >= m_SpawnTimer)
	{
		if (m_SpawnTimer <= 2.50f && m_iSpawnCount == 1)
		{
			m_iSpawnCount = 2;
		}

		for (int i = 0; i < m_iSpawnCount; i++)
		{
			//Random x pos
			float xSpawn = float(rand() % 18);
			xSpawn -= 9.0f;

			//Create and initialise new enemy
			cEnemy* newEnemy = new cEnemy("Resources/Models/Pug/Dog 1.obj", camera);
			newEnemy->SetTranslate(vec3(xSpawn, 0.0f, -70.0f));
			newEnemy->SetVelocity(m_v3EnemyVelocity);
			m_vecEnemies.push_back(newEnemy);

			//Decrease next spawn time, until limit 
			if ((m_SpawnTimer >= 1.5f))
			{
				m_SpawnTimer -= 0.2f;
			}

			//Increase next enemy velocity, until limit
			if (m_v3EnemyVelocity.z <= 10.0f)
			{
				m_v3EnemyVelocity.z += 0.2f;
			}

			m_fLastSpawn = 0.0f;
		}
	}
}

void cLevel::UpdateLives()
{
	//Check every enemy
	for (auto i = m_vecEnemies.begin(); i < m_vecEnemies.end(); i++)
	{
		//If behind tank
		if ((*i)->GetTranslate().z >= m_pTank->GetTranslate().z)
		{
			//Decrease lives
			m_iLives--;

			//Delete and erase enemy that gets past
			delete (*i);
			m_vecEnemies.erase(i);

			//End game if lives less than 0
			if (m_iLives <= 0)
			{
				//Show results
				m_bDisplayResults = true;
			}

			break;
		}
	}
}
