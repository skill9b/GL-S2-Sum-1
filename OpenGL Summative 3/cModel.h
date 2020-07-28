#pragma once
#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes

#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "cModelMesh.h"
#include "cCamera.h"
#include "ShadeLoader.h"

class cModel
{
public:
	cModel(string path, cCamera* camera);
	~cModel();

	void Render();
	void Update(float _deltaTime);

	//Setters
	void SetTranslate(glm::vec3 _newTranslate);
	void SetScale(glm::vec3 _newScale);
	void SetScaleFactor(float _newFactor);
	void SetRotate(glm::vec3 _newRotation);
	void SetRotateAngle(float _newAngle);

	//Getters
	glm::vec3 GetTranslate();
	float GetAngle();
	glm::vec3 GetRotation();

private:
	GLuint m_gluiProgram;
	cCamera* m_pCamera;
	glm::vec3 m_v3Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_v3Translate = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_v3Rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_glfScaleFactor = 1.0f;
	float m_glfAngle = 0.0f;



	vector<cModelMesh> m_vecMeshes;
	string m_strDirectory;
	vector<MeshTexture> m_vecTexturesLoaded; //Stores loaded texturs, makes sure textures aren't loaded twice

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);

	cModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLuint LoadTextureFromFile(const char* path, string directory);
};

