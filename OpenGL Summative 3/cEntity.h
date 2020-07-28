#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "cMesh.h"
#include "cCamera.h"
#include "cCubeMap.h"

using namespace glm;
class cEntity
{
protected:
    cMesh* m_pMesh;
    mat4 m_m4Translation;
    mat4 m_m4Rotation;
    mat4 m_m4Scale;
    mat4 m_m4Model;
   
    vec3 m_v3Position; //Translation vector
    vec3 m_v3Rotation;
    GLfloat m_glfAngle;
    vec3 m_v3Scale;
    GLfloat m_glfScaleFactor;
    vec2 m_v2Size;

public:
    //Constructor
    cEntity();
    //Destructor
    ~cEntity();

    //Setters
    void SetTranslation(vec3 _v3Position);
    void SetRotation(vec3 _v3RotationAxisZ, float _fRotationAngleRadians);
    void SetScale(vec3 _v3Scale, float _fFactor);

    //Getters
    mat4 GetTranslation();
	mat4 GetRotation();
	mat4 GetScale();
	mat4 GetModel();
    vec3 GetTranslate();
 
    void Update(float _deltaTime);
    void Render(GLuint _program, cCamera* _camera);
    void Render(GLuint _program, cCamera* _camera, GLuint _VAO, GLuint _indiceCount);
    void Render(GLuint _program, cCamera* _camera, GLuint _VAO, GLuint _indiceCount, cCubeMap* _cubeMap);
    void SetModelMatrix();
    void Intitialise(vector<Vertex2D> _vertices, vector<GLuint> _indices, 
                     int _frames, string _filename, vec3 _v3translation,
                     vec3 _v3RotationAxisZ, float _fRotationAngleRadians,
                     vec3 _v3Scale, float _fFactor, int _fps, float width, float height);
     
};

