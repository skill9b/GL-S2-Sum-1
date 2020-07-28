#include "cModel.h"

cModel::cModel(string path, cCamera* camera)
{
	//Create shader
	this->m_gluiProgram = ShaderLoader::CreateProgram("Resources/Shaders/Model.vs", "Resources/Shaders/Model.fs");
	this->m_pCamera = camera;

	//Load models
	this->LoadModel(path);
}

cModel::~cModel()
{
}

void cModel::Render()
{
	//Calculate pvm
	mat4 PVM = mat4();
	mat4 Translation = translate(mat4(), m_v3Translate);
	mat4 Rotation = rotate(mat4(), radians(m_glfAngle), m_v3Rotation);
	mat4 Scale = scale(mat4(), m_v3Scale * m_glfScaleFactor);
	
	PVM = Translation * Rotation * Scale;

	//Use pvm to render each model
	for (GLuint i = 0; i < this->m_vecMeshes.size(); i++)
	{
		this->m_vecMeshes[i].Render(m_pCamera, m_gluiProgram, PVM);
	}
}

void cModel::Update(float _deltaTime)
{
}

void cModel::LoadModel(string path)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->m_strDirectory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->ProcessNode(scene->mRootNode, scene);
}

void cModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_vecMeshes.push_back(this->ProcessMesh(mesh, scene));
	}


	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

cModelMesh cModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<MeshTexture> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<MeshTexture> specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return cModelMesh(vertices, indices, textures);
}

vector<MeshTexture> cModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<MeshTexture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < m_vecTexturesLoaded.size(); j++)
		{
			if (m_vecTexturesLoaded[j].path == str)
			{
				textures.push_back(m_vecTexturesLoaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			MeshTexture texture;
			texture.id = LoadTextureFromFile(str.C_Str(), this->m_strDirectory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->m_vecTexturesLoaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

GLuint cModel::LoadTextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

void cModel::SetTranslate(glm::vec3 _newTranslate)
{
	m_v3Translate = _newTranslate;
}

void cModel::SetScale(glm::vec3 _newScale)
{
	m_v3Scale = _newScale;
}

void cModel::SetScaleFactor(float _newFactor)
{
	m_glfScaleFactor = _newFactor;
}

void cModel::SetRotate(glm::vec3 _newRotation)
{
	m_v3Rotation = _newRotation;
}

void cModel::SetRotateAngle(float _newAngle)
{
	m_glfAngle = _newAngle;
}

glm::vec3 cModel::GetTranslate()
{
	return m_v3Translate;
}

float cModel::GetAngle()
{
	return m_glfAngle;
}

glm::vec3 cModel::GetRotation()
{
	return m_v3Rotation * m_glfAngle;
}

