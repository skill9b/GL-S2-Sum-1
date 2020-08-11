#include "cEntity.h"

cEntity::cEntity()
{
	m_pMesh = new cMesh();
	m_m4Translation = translate(mat4(), vec3(0.0f, 0.0f, 0.0f));
	m_m4Rotation = rotate(mat4(), radians(0.0f), vec3(0.0f, 0.0f, 0.0f));
	m_m4Scale = scale(mat4(), vec3(0.0f, 0.0f, 0.0f));
	m_m4Model = mat4();

	m_v3Position = vec3(0.0f, 0.0f, 0.0f);
	m_v3Rotation = vec3(0.0f, 0.0f, 0.0f);
	m_glfAngle = 90.0f;
	m_v3Scale = vec3(0.0f, 0.0f, 0.0f);
	m_glfScaleFactor = 0.0f;
	m_v2Size = vec2(0.0f, 0.0f);
}

cEntity::~cEntity()
{
	delete m_pMesh;
	m_pMesh = 0;
}

void cEntity::SetTranslation(vec3 _v3Position)
{
	m_v3Position = _v3Position;
}

void cEntity::SetRotation(vec3 _v3RotationAxisZ, float _fRotationAngleRadians)
{
	m_v3Rotation = _v3RotationAxisZ;
	m_glfAngle = _fRotationAngleRadians;
}

void cEntity::SetScale(vec3 _v3Scale, float _fFactor)
{
	m_v3Scale = _v3Scale;
	m_glfScaleFactor = _fFactor;
}

mat4 cEntity::GetTranslation()
{
	return m_m4Translation;
}

mat4 cEntity::GetRotation()
{
	return m_m4Rotation;
}

mat4 cEntity::GetScale()
{
	return m_m4Scale;
}

mat4 cEntity::GetModel()
{
	return m_m4Model;
}

vec3 cEntity::GetTranslate()
{
	return m_v3Position;
}

cMesh* cEntity::GetMesh()
{
	return m_pMesh;
}

void cEntity::Update(float _deltaTime)
{
	SetModelMatrix();
	m_pMesh->Update(_deltaTime);
}

void cEntity::Render(GLuint _program, cCamera* _camera)
{
	glUseProgram(_program);

	glBindVertexArray(m_pMesh->GetVAO());

	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_pMesh->GetSprite()->GetTexID());
	glUniform1i(glGetUniformLocation(_program, "tex0"), 0);

	//Uniforms
	GLuint modelLoc = glGetUniformLocation(_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(m_m4Model));

	GLuint viewLoc = glGetUniformLocation(_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(_camera->GetViewMatrix()));
	
	GLuint projLoc = glGetUniformLocation(_program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(_camera->GetProjectionMatrix()));

	GLuint camPos = glGetUniformLocation(_program, "camPos");
	glUniform3fv(camPos, 1, value_ptr(_camera->GetCamPos()));

	glDrawElements(GL_TRIANGLES, this->m_pMesh->GetIndices().size(), GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void cEntity::Render(GLuint _program, cCamera* _camera, GLuint _VAO, GLuint _indiceCount)
{
	glUseProgram(_program);

	glBindVertexArray(_VAO);
	
	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_pMesh->GetSprite()->GetTexID());
	glUniform1i(glGetUniformLocation(_program, "tex0"), 0);

	//Uniforms
	GLuint modelLoc = glGetUniformLocation(_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(m_m4Model));
	
	GLuint viewLoc = glGetUniformLocation(_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(_camera->GetViewMatrix()));
	
	GLuint projLoc = glGetUniformLocation(_program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(_camera->GetProjectionMatrix()));

	GLuint camPos = glGetUniformLocation(_program, "camPos");
	glUniform3fv(camPos, 1, value_ptr(_camera->GetCamPos()));

	glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void cEntity::Render(GLuint _program, cCamera* _camera, GLuint _VAO, GLuint _indiceCount, cCubeMap* _cubeMap)
{
	glUseProgram(_program);

	glBindVertexArray(_VAO);

	//Textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap->GetTextureID());
	glUniform1i(glGetUniformLocation(_program, "skybox"), 1);

	//Uniforms
	GLuint modelLoc = glGetUniformLocation(_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(m_m4Model));

	GLuint PVMLoc = glGetUniformLocation(_program, "pvm");
	glm::mat4 PVM = _camera->GetPVMatrix() * m_m4Model;
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, value_ptr(PVM));
	
	GLuint NormCalcLoc = glGetUniformLocation(_program, "normcalc");
	glm::mat3 NormCalc = mat3(transpose(inverse(m_m4Model)));
	glUniformMatrix3fv(NormCalcLoc, 1, GL_FALSE, value_ptr(NormCalc));

	GLuint CamPosLolc = glGetUniformLocation(_program, "camPos");
	glUniform3fv(CamPosLolc, 1, value_ptr(_camera->GetCamPos()));

	glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

void cEntity::SetModelMatrix()
{
	//Calculate model
	m_m4Translation = translate(mat4(), m_v3Position);
	m_m4Rotation = rotate(mat4(), radians(m_glfAngle), m_v3Rotation);
	m_m4Scale = scale(mat4(), m_v3Scale * m_glfScaleFactor);

	m_m4Model = m_m4Translation * m_m4Rotation * m_m4Scale;
}

void cEntity::Intitialise(vector<Vertex2D> _vertices, vector<GLuint> _indices, int _frames, string _filename, vec3 _v3translation, vec3 _v3RotationAxisZ, float _fRotationAngleRadians, vec3 _v3Scale, float _fFactor, int _fps, float width, float height)
{
	m_v2Size.y = width;
	m_v2Size.x = height;

	//Initialise model
	SetRotation(_v3RotationAxisZ, _fRotationAngleRadians);
	SetTranslation(_v3translation);
	SetScale(_v3Scale, _fFactor);
	SetModelMatrix();

	//Initialise Mesh
	m_pMesh->InitialiseSprite(_frames, _filename, _fps);
	m_pMesh->InitialiseMesh(_vertices, _indices);
}






