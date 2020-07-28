#include "cText.h"


cText::cText(string _text, string _font, vec2 _pos,
	vec3 _colour, float _scale)
	: m_strText(""), m_glfScale(0), m_vec3Color(0), m_vec2Position(0),
	m_gliVAO(0), m_gliVBO(0), m_gliProgram(0), m_m4Projection(0)
{
	//Initialise 
	m_strText = _text;
	SetColor(_colour);
	SetScale(_scale);
	SetPosition(_pos);

	GLfloat halfWidth = (GLfloat)SCR_WIDTH * 0.5f;
	GLfloat halfHeight = (GLfloat)SCR_HEIGHT * 0.5f;

	//Camera
	m_m4Projection = ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);

	//Program
	m_gliProgram = ShaderLoader::CreateProgram("Resources/Shaders/Text.vs",
											   "Resources/Shaders/Text.fs");

	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft) != 0)
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(ft, _font.c_str(), 0, &face) != 0)
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate each char
	for (GLubyte character = 0; character < 128; character++)
	{
		if (FT_Load_Char(face, character, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		GLuint texture = GenerateTexture(face);

		FontChar fontChar = {
		texture,
		ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		(GLuint)face->glyph->advance.x };

		m_mapCharacters.insert(std::pair<GLchar, FontChar>(character, fontChar));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &m_gliVAO);
	glGenBuffers(1, &m_gliVBO);

	glBindVertexArray(m_gliVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gliVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	//
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


cText::~cText()
{
}

void cText::Render()
{
	glUseProgram(m_gliProgram);
	glUniform3f(glGetUniformLocation(m_gliProgram, "textColor"), m_vec3Color.x, m_vec3Color.y, m_vec3Color.z);
	glUniformMatrix4fv(glGetUniformLocation(m_gliProgram, "proj"), 1, GL_FALSE, value_ptr(m_m4Projection));
	glBindVertexArray(m_gliVAO);

	//Iterate through all characters
	vec2 textPos = m_vec2Position;

	for (string::const_iterator character = m_strText.begin(); character != m_strText.end(); character++)
	{
		FontChar fontChar = m_mapCharacters[*character];
		GLfloat xpos = textPos.x + fontChar.Bearing.x * m_glfScale;
		GLfloat ypos = textPos.y - (fontChar.Size.y - fontChar.Bearing.y) * m_glfScale;
		GLfloat charWidth = fontChar.Size.x * m_glfScale;
		GLfloat charHeight = fontChar.Size.y * m_glfScale;

		//Update VBO for each character
		GLfloat vertices[6][4] =
		{
			{ xpos, ypos + charHeight, 0.0f, 0.0f }, { xpos, ypos, 0.0f, 1.0f },
			{ xpos + charWidth, ypos, 1.0f, 1.0f }, { xpos, ypos + charHeight, 0.0f, 0.0f },
			{ xpos + charWidth, ypos, 1.0f, 1.0f }, { xpos + charWidth, ypos + charHeight, 1.0f, 0.0f }
		};

		glBindBuffer(GL_ARRAY_BUFFER, m_gliVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		//Render glyph texture over the quad
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fontChar.TextureID);
		glUniform1i(glGetUniformLocation(m_gliProgram, "tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Advance cursors for the next glyph
		textPos.x += (fontChar.Advance >> 6) * m_glfScale; 
	}

	//Unbind everything
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void cText::SetText(string _newText)
{
	m_strText = _newText;
}

void cText::SetColor(vec3 _newColor)
{
	m_vec3Color = _newColor;
}

void cText::SetScale(float _newScale)
{
	m_glfScale = _newScale;
}

void cText::SetPosition(vec2 _newPosition)
{
	m_vec2Position = _newPosition;
}

GLuint cText::GenerateTexture(FT_Face _face)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RED,
		_face->glyph->bitmap.width,
		_face->glyph->bitmap.rows,
		0,
		GL_RED, GL_UNSIGNED_BYTE,
		_face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}