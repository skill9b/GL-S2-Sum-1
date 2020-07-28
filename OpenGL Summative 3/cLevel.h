#pragma once
#include "cEntity.h"
#include "cAudio.h"
#include "cInput.h"
#include "cText.h"
#include <string>
#include "Sphere.h"
#include "cCubeMap.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cBullet.h"
#include "vector"

class cLevel
{
private:
	//Utility
	cAudio* m_pAudioManager;
	cInput* m_pInputManager;
	Sphere* m_pSphereMesh;

	//Game Variables
	bool m_bDisplayResults;
	bool m_bEndGame;
	int m_iLives;
	float m_fFireTimer;

	//UI
	cText* m_pScore;
	cText* m_pLives;
	cText* m_pInstructions;
	cText* m_pLoading;
	int m_iScore;

	//Game objects
	std::vector<cBullet*> m_vecBullets;
	std::vector<cEnemy*> m_vecEnemies;
	cPlayer* m_pTank;

	//Wave Variables
	int m_iSpawnCount;
	float m_fLastSpawn;
	float m_SpawnTimer;
	vec3 m_v3EnemyVelocity;

public:

	//Member Functions
	cLevel();  //Constructor
	~cLevel(); //Destructor
	void Initialise(cCamera* _camera);
	void Update(float _deltaTime, cInput* _inputManager, cCamera* _camera);
	void UpdateLives();
	void Render(cCamera* _camera, GLuint _reflectionProgram, cCubeMap* _cubeMap);
	bool GetInGameState();
	void RestartInput(cInput* _inputManager);
	void RestartGame();

	//Bullet Functions
	void DeleteBullets();
	void MoveBullets(float _deltaTime);
	void BulletCollisions();
	void RenderBullets(cCamera* _camera, GLuint _reflectionProgram, cCubeMap* _cubeMap);
	void UpdateBullets(float _deltaTime);
	void CheckShoot(float _deltaTime);

	//Enemy Functions
	void MoveEnemies(float _deltaTime);
	void UpdateEnemies(float _deltaTime);
	void RenderEnemies();
	void SpawnEnemies(float _deltaTime, cCamera* camera);

};

