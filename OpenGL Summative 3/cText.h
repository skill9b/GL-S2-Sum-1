#pragma once
#include <SOIL.h>

#include <glew.h>
#include <freeglut.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>
#include <vector>

#include "ShadeLoader.h"
#include "Global.h"
using namespace std;
using namespace glm;
struct FontChar
{
	GLuint TextureID = 0; 
	ivec2 Size = vec2(0.0f, 0.0f);
	ivec2 Bearing = vec2(0.0f, 0.0f);
	GLuint Advance = 0;
};

class cText
{
public:
	cText(
		string _text,
		string _font,
		vec2 _pos,
		vec3 _color = vec3(1.0f, 1.0f, 1.0f),
		float _scale = 1.0f);
		
	~cText();

	void Render();
	void SetText(string _newText);
	void SetColor(vec3 _newColor);
	void SetScale(float _newScale);
	void SetPosition(vec2 _newPosition);

private:
	GLuint GenerateTexture(FT_Face _face);

	string m_strText;
	GLfloat m_glfScale;
	vec3 m_vec3Color;
	vec2 m_vec2Position;

	GLuint m_gliVAO, m_gliVBO, m_gliProgram;
	mat4 m_m4Projection;
	map<GLchar, FontChar> m_mapCharacters;
};

