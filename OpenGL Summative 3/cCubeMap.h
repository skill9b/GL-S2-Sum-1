#pragma once

#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <iostream>
#include <fmod.hpp>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "cCamera.h"
#include "ShadeLoader.h"

class cCubeMap
{
public:
    cCubeMap(cCamera* _camera);
    ~cCubeMap();

    void Update();
    void Render();
    GLuint GetTextureID();

private:
    GLuint m_gliProgram;
    cCamera* m_pCamera;

    glm::mat4 m_m4MVP;

    GLuint m_gluiVAO;
    GLuint m_gluiEBO;
    GLuint m_gluiVBO;

    GLuint m_gliTextureID;
};
