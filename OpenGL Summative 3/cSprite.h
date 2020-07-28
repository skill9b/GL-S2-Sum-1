#pragma once
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <SOIL.h>
#include <iostream>

using namespace std;

class cSprite
{   
private:

    GLuint m_gliTextureID;
    GLfloat m_glfTexturePosX;
    int m_iTotalFrames;
    string m_strFileName;
    int m_iCurrentFrame;
    float m_fSecondsPassed;
    int m_iFPS;
    float m_fTimeStep;

public:
    cSprite();
    ~cSprite();

    void Update(float _deltaTime);
    void Intitialise(string filename, int frames, int _fps);
    GLuint GenerateTexture();
    GLuint GetTexID();
    int GetTotalFrames();
    GLfloat GetTexPosX();
};

