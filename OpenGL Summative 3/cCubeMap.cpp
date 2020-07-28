#include "cCubeMap.h"

cCubeMap::cCubeMap(cCamera* _camera)
{
	m_pCamera = _camera;
	m_gliProgram = ShaderLoader::CreateProgram("Resources/Shaders/CubeMap.vs",
		"Resources/Shaders/CubeMap.fs");

	std::string textures[6]{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg" };

	int width, height;
	unsigned char* image;

	glGenTextures(1, &m_gliTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_gliTextureID);


	// Generates all the texturs
	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Textures/CubeMap/";
		fullPathName.append(textures[i]);
		image = SOIL_load_image(fullPathName.c_str(),
			&width,
			&height,
			0,
			SOIL_LOAD_RGBA);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image);
		SOIL_free_image_data(image);
	}


	// Makes it look all smooth
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	float fSize = 0.5f;
	GLfloat CubeMapVertices[]
	{
		//Front
		-fSize,	 fSize,	 fSize,
		-fSize,	-fSize,	 fSize,
		 fSize,	-fSize,	 fSize,
		 fSize,	 fSize,	 fSize,

		//Back
		-fSize,  fSize, -fSize,
		-fSize, -fSize, -fSize,
		 fSize, -fSize, -fSize,
		 fSize,  fSize, -fSize,

		//Left
		-fSize,  fSize, -fSize,
		-fSize, -fSize, -fSize,
		-fSize, -fSize,  fSize,
		-fSize,  fSize,  fSize,

	    //Right
	     fSize,  fSize,  fSize,
	     fSize, -fSize,  fSize,
	     fSize, -fSize, -fSize,
	     fSize,  fSize, -fSize,

		//Top
	    -fSize,  fSize, -fSize,
	    -fSize,  fSize,  fSize,
		 fSize,  fSize,  fSize,
		 fSize,  fSize, -fSize,

		//Bottom
	    -fSize, -fSize,  fSize,
	    -fSize, -fSize, -fSize,
		 fSize, -fSize, -fSize,
		 fSize, -fSize,  fSize,
	};

	GLuint CubeMapIndices[] =
	{
		0,	2,	1,
		0,	3,	2,

		7,	5,	6,
		7,	4,	5,

		8,	10,	9,
		8,	11,	10,

		12,	14,	13,
		12,	15,	14,

		16,	18,	17,
		16,	19,	18,

		20,	22,	21,
		20,	23,	22,
	};


	// Creates the VBO, EBO and VAO
	glGenVertexArrays(1, &m_gluiVAO);
	glBindVertexArray(m_gluiVAO);

	glGenBuffers(1, &m_gluiEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeMapIndices), CubeMapIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_gluiVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gluiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMapVertices), CubeMapVertices, GL_STATIC_DRAW);

	// Makes the points Point things
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

cCubeMap::~cCubeMap()
{

}


void cCubeMap::Update()
{
	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(200.0f, 200.0f, 200.0f));
	m_m4MVP = m_pCamera->GetPVMatrix() * model;
}


void cCubeMap::Render()
{
	glUseProgram(m_gliProgram);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//Uniforms
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_gliTextureID);

	GLuint CubeShaderUniform = glGetUniformLocation(m_gliProgram, "cubeSampler");
	glUniform1i(CubeShaderUniform, 0);

	GLuint MVPUniform = glGetUniformLocation(m_gliProgram, "MVP");
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, glm::value_ptr(m_m4MVP));

	//Draw
	glBindVertexArray(m_gluiVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

GLuint cCubeMap::GetTextureID()
{
	return m_gliTextureID;
}
