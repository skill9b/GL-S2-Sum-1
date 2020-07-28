#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <assimp/Importer.hpp>

// EDIT
#include "Global.h"
#include "cCamera.h"
// EDIT END

struct Vertex {

	glm::vec3 Position;  // Position	
	glm::vec3 Normal;	 // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id;
	string type;
	aiString path;
};

class cModelMesh
{
public:
	cModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures);
	void Render(cCamera* camera, GLuint program, glm::mat4 model);

private:
	/*  Mesh Data  */
	vector<Vertex> m_vecVertices;
	vector<GLuint> m_vecIndices;
	vector<MeshTexture> m_vecTextures;

	/*  Render data  */
	GLuint m_gluiVAO, m_gluiVBO, m_gluiEBO;

	void SetUpMesh();
};

