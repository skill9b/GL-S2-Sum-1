#pragma once
#include "cSprite.h"
#include <vector>

using namespace glm;
struct Vertex2D {
	vec3 Position;
	vec3 Color;
	vec2 TexCoords;
};

class cMesh
{
private:
	vector<Vertex2D> m_vecVertices;
	vector<GLuint> m_vecIndices;
	GLuint m_gliVAO, m_gliVBO, m_gliEBO;
	cSprite* m_pSprite;

public:
	cMesh(); //Constructor
	~cMesh(); //Destructor

	void Update(float _deltaTime);
	void InitiliseTextureCoords();
	void InitialiseSprite(int _frames, string _filename, int _fps);
	void InitialiseMesh(vector<Vertex2D> _vertices, vector<GLuint> _indices);
	GLuint GetVAO();
	vector<GLuint> GetIndices();
	cSprite* GetSprite();


};

