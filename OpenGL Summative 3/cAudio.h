#pragma once
#include <freeglut.h>
#include <iostream>
#include <vector>
#include <fmod.hpp>
using namespace FMOD;
using namespace std;

class cAudio
{
public:
	System* m_pAudioSystem;
	Sound* m_pBGM;
	Sound* m_pfxBallCollides;
	
	cAudio();
	~cAudio();

	void Initialise();
	void Update();
	void PlaySound(Sound*_SoundToPlay);
};

