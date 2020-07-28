#include "cAudio.h"

cAudio::cAudio()
{
	m_pAudioSystem = 0;
	m_pfxBallCollides= 0;
	m_pBGM = 0;
}

cAudio::~cAudio()
{
	m_pfxBallCollides->release();
	m_pBGM->release();
	m_pAudioSystem->release();

	//delete m_pAudioSystem;
	//delete m_pfxBallCollides;
	//delete m_pBGM;
	//
	//m_pAudioSystem = 0;
	//m_pfxBallCollides = 0;
	//m_pBGM = 0;
}

void cAudio::Initialise()
{
	FMOD_RESULT result = FMOD_OK;
	result = System_Create(&m_pAudioSystem);

	if (result != FMOD_OK)
	{
		cout << "Error creating audio system" << endl;
		system("pause");
	}

	result = m_pAudioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);

	if (result != FMOD_OK)
	{
		cout << "Error initialising audio system" << endl;
		system("pause");
	}

	result = m_pAudioSystem->createSound(
		"Resources/Audio/BGM.mp3",
		FMOD_LOOP_NORMAL,
		0,
		&m_pBGM
		);

	if (result != FMOD_OK)
	{
		cout << "Error loading BGM" << endl;
		system("pause");
	}

	result = m_pAudioSystem->createSound(
		"Resources/Audio/Bounce.wav",
		FMOD_DEFAULT,
		0,
		&m_pfxBallCollides
		);

	if (result != FMOD_OK)
	{
		cout << "Error loading bounce effect" << endl;
		system("pause");
	}
}

void cAudio::Update()
{
	m_pAudioSystem->update();
}

void cAudio::PlaySound(Sound*_SoundToPlay)
{
	FMOD_RESULT result = FMOD_OK;
	result = m_pAudioSystem->playSound(_SoundToPlay, 0, false, 0);
	if (result != FMOD_OK)
	{
		cout << "Error playing sound" << endl;
		system("pause");
	}
}

