#include "cModelMesh.h"

cModelMesh::cModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures)
{
	this->m_vecVertices = vertices;
	this->m_vecIndices = indices;
	this->m_vecTextures = textures;
	this->SetUpMesh();
}

void cModelMesh::Render(cCamera* camera, GLuint program, mat4 model)
{
	glUseProgram(program);

	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->m_vecTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->m_vecTextures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();
		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->m_vecTextures[i].id);
	}

	// EDIT
	glm::mat4 pvm = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model; 
	GLint pvmLoc = glGetUniformLocation(program, "PVM");
	glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(pvm));

	GLuint camPos = glGetUniformLocation(program, "camPos");
	glUniform3fv(camPos, 1, value_ptr(camera->GetCamPos()));

	GLuint NormCalcLoc = glGetUniformLocation(program, "normcalc");
	glm::mat3 NormCalc = mat3(transpose(inverse(model)));
	glUniformMatrix3fv(NormCalcLoc, 1, GL_FALSE, value_ptr(NormCalc));

	// EDIT END

	// Draw mesh
	glBindVertexArray(this->m_gluiVAO);
	glDrawElements(GL_TRIANGLES, this->m_vecIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->m_vecTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void cModelMesh::SetUpMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->m_gluiVAO);
	glGenBuffers(1, &this->m_gluiVBO);
	glGenBuffers(1, &this->m_gluiEBO);

	glBindVertexArray(this->m_gluiVAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->m_gluiVBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->m_vecVertices.size() * sizeof(Vertex), &this->m_vecVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_gluiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_vecIndices.size() * sizeof(GLuint), &this->m_vecIndices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
