#include "Sphere.h"

Sphere::Sphere()
{
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	float phi = 0;
	float theta = 0;

	//Calculate vertices
	float vertices[(sections) * (sections)*vertexAttrib];
	int offset = 0;

	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cosf(phi) * sinf(theta);
			float y = cosf(theta);
			float z = sinf(phi) * sinf(theta);

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (double)(M_PI / (double)(sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}

	//Calculate Indices
	GLuint indices[(sections) * (sections)*indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
		}
	}

	//Generate VAO
	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_gluiVAO);
	glBindVertexArray(m_gluiVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	m_gluiIndiceCount = sizeof(indices) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}



Sphere::~Sphere()
{
}

void Sphere::Render()
{
	glBindVertexArray(m_gluiVAO);
	glDrawElements(m_iDrawType, m_gluiIndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint Sphere::GetVAO()
{
	return m_gluiVAO;
}

GLuint Sphere::GetIndice()
{
	return m_gluiIndiceCount;
}
