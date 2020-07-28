#include "cMesh.h"

#define PI 3.14159265358979323846

cMesh::cMesh()
{
    m_gliVAO = 0;
    m_gliEBO = 0;
    m_gliVBO = 0;
    m_pSprite = new cSprite();
}

cMesh::~cMesh()
{
    delete m_pSprite;
    m_pSprite = 0;
}


void cMesh::Update(float _deltaTime)
{
    //m_pSprite->Update(_deltaTime);
}

void cMesh::InitiliseTextureCoords()
{
    for (unsigned int i = 0; i < m_vecVertices.size(); i++)
    {
        m_vecVertices[i].TexCoords.x = m_vecVertices[i].TexCoords.x / m_pSprite->GetTotalFrames();
    }
}

void cMesh::InitialiseSprite(int _frames, string _filename, int _fps)
{
    m_pSprite->Intitialise(_filename, _frames, _fps);
}

void cMesh::InitialiseMesh(vector<Vertex2D> _vertices, vector<GLuint> _indices)
{
    this->m_vecVertices = _vertices;
    this->m_vecIndices = _indices;
    
    InitiliseTextureCoords();

    glGenVertexArrays(1, &m_gliVAO);
    glGenBuffers(1, &m_gliVBO);
    glGenBuffers(1, &m_gliEBO);

    glBindVertexArray(m_gliVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gliVBO);

    glBufferData(GL_ARRAY_BUFFER, m_vecVertices.size() * sizeof(Vertex2D), &m_vecVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gliEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndices.size() * sizeof(unsigned int),
                 &m_vecIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, Color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, TexCoords));

    glBindVertexArray(0);
}

GLuint cMesh::GetVAO()
{
	return m_gliVAO;
}

vector<GLuint> cMesh::GetIndices()
{
    return m_vecIndices;
}

cSprite* cMesh::GetSprite()
{
    return m_pSprite;
}
