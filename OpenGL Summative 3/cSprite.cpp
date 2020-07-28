#include "cSprite.h"

cSprite::cSprite()
{
	m_gliTextureID = 0;
	m_glfTexturePosX = 0.0f;
	m_iTotalFrames = 0;
	m_strFileName = "";
	m_fSecondsPassed = 0;
	m_iCurrentFrame = 0;
	m_iFPS = 1; 
	m_fTimeStep = 1.0f / (float)m_iFPS;
}

cSprite::~cSprite()
{
}

void cSprite::Update(float _deltaTime)
{
	
	m_fSecondsPassed += _deltaTime;

	//Update sprite every second, 1 frame
	if (m_fSecondsPassed >= m_fTimeStep)
	{
		m_iCurrentFrame++;
		m_fSecondsPassed -= m_fTimeStep;
		if (m_iCurrentFrame >= m_iTotalFrames)
		{
			//Reset at end of animation
			m_iCurrentFrame = 0;
		}
	}

	m_glfTexturePosX = ( (float)m_iCurrentFrame * (1.0f / (float)m_iTotalFrames) );
}

void cSprite::Intitialise(string filename, int frames, int _fps)
{
	m_strFileName = filename;
	m_iTotalFrames = frames; //Use frames to initialise tex coords in mesh	
	m_gliTextureID = GenerateTexture();
	m_iFPS = _fps;
	m_fTimeStep = 1.0f / (float)m_iFPS;
}

GLuint cSprite::GenerateTexture()
{
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image(m_strFileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;;
}

GLuint cSprite::GetTexID()
{
	return m_gliTextureID;
}

int cSprite::GetTotalFrames()
{
	return m_iTotalFrames;
}

GLfloat cSprite::GetTexPosX()
{
	return m_glfTexturePosX;
}
