#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////

// Sampler value for the next texture.
GLint TiledTexture::m_nextSampler = 0;

//////////////////////////////////////////////////////////////////////////


TiledTexture::TiledTexture(const GLuint& program, const std::string& fileName, const std::string& textureSamplerName,
	int surfaceWidth, int surfaceHeight, const std::vector<GLfloat>& vertices, const TexturePresence& texturePresence)
	: m_fileName(fileName), m_texture{}, m_uTextureSampler(-1), m_currentSampler(m_nextSampler++),
	  m_textureCoordBuffer{}, m_textureWidth{}, m_textureHeight{}, 
	  m_surfaceWidth(surfaceWidth), m_surfaceHeight(surfaceHeight), m_texturePresence(texturePresence)
{
	if (!initialize(program, textureSamplerName, vertices))
	{
		throw EXCEPTION_FMT(L"Failed to initialize texture: %S", fileName.c_str());
	}
}

TiledTexture::~TiledTexture()
{
	if (0 != m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_texture);
	}

	if (0 != m_textureCoordBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_textureCoordBuffer);
	}
}

bool TiledTexture::initialize(const GLuint& program, const std::string& textureSamplerName, const std::vector<GLfloat>& vertices)
{
	if (m_fileName.empty())
	{
		std::cerr << "Empty texture file name\n";
		return false;
	}
	else if (   m_surfaceWidth  <= 0 
		     || m_surfaceHeight <= 0)
	{
		std::cerr << "Invalid surface width (" << m_surfaceWidth << ") and/or height (" << m_surfaceHeight << ") value\n";
		return false;
	}
	else if (0 == program)
	{
		std::cerr << __FUNCTION__ << "invalid program value\n";
		return false;
	}

	// Set texture sampler uniform.

	m_uTextureSampler = glGetUniformLocation(program, textureSamplerName.c_str());
	if (-1 == m_uTextureSampler)
	{
		std::cerr << "Failed to get the uSampler uniform location\n";
		return false;
	}

	glUniform1i(m_uTextureSampler, m_currentSampler);

	// NOTE: this should be done after setting the texture sampler uniform for the fragment shader.

	// Create and fill texture data buffer.

	glGenTextures(1, &m_texture);

	glActiveTexture(GL_TEXTURE0 + m_currentSampler);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Read texture data from the file.

	auto del = [](unsigned char* pBuff) {
		SOIL_free_image_data(pBuff);
	};

	std::unique_ptr<unsigned char[], decltype(del)> spTextureData(
		SOIL_load_image(m_fileName.c_str(), &m_textureWidth, &m_textureHeight, 0, SOIL_LOAD_RGB));
	if (!spTextureData)
	{
		std::cerr << "Failed to load terrain texture " << m_fileName.c_str() << ": " << SOIL_last_result() << '\n';
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_textureWidth, m_textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, spTextureData.get());

	// Generate texture coordinates.

	std::vector<glm::vec2> textureCoords;

	const size_t VERTEX_COUNT = vertices.size();

	textureCoords.resize(VERTEX_COUNT / 3);    // vertex coordinates collection contains 3 coordinates per vertex

	size_t currentTexCoord = {};

	// Number of tiles to use for each dimension.
	const GLfloat TILE_COUNT = 20.0f;

	for (size_t i = 0; i < VERTEX_COUNT; i += 3)
	{
		textureCoords[currentTexCoord++] = glm::vec2(vertices[i] / (m_surfaceWidth / TILE_COUNT), 
			vertices[i + 2] / (m_surfaceHeight / TILE_COUNT));
	}

	// Create and fill texture coordinates buffer.

	glGenBuffers(1, &m_textureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * textureCoords.size(), &textureCoords[0], GL_STATIC_DRAW);

	const GLuint attrTextureCoordinate = 2;
	glVertexAttribPointer(attrTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrTextureCoordinate);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0 + m_currentSampler);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

GLfloat TiledTexture::getTexturePresencePercent(unsigned char height) const
{
	return m_texturePresence.getTexturePresencePercent(height);
}
