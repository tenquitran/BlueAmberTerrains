#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Terrain::Terrain(GLfloat scaleFactor)
	: m_heightmap(scaleFactor), m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_unMvp(-1), m_color{}, 
	  m_minHeightScaled{}, m_maxHeightScaled{}
{
}

Terrain::~Terrain()
{
	if (0 != m_color)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_color);
	}

	if (0 != m_index)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_index);
	}

	if (0 != m_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
	}

	if (0 != m_vao)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}
}

bool Terrain::loadHeightmapFromFile(const CAtlString& filePath)
{
	return m_heightmap.loadFromFile(filePath);
}

bool Terrain::initialize()
{
	// The terrain cannot be initialized until loading of the heightmap data.
	if (m_heightmap.isEmpty())
	{
		std::cerr << "Cannot initialize terrain: no heightmap data\n";
		assert(false); return false;
	}

	// Initialize the program wrapper.

	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "shaders\\terrain.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\terrain.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colors;    // colors for height-based coloring

	if (!generateMeshData(vertices, indices, colors))
	{
		return false;
	}

	m_indexCount = indices.size();

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Generate VBO and fill it with the data. Fill the shader attributes.

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	const GLuint attrVertexPosition = 0;
	glVertexAttribPointer(attrVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrVertexPosition);

	glGenBuffers(1, &m_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_color);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	const GLuint attrVertexColor = 1;
	glVertexAttribPointer(attrVertexColor, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrVertexColor);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const GLuint program = m_spProgram->getProgram();

	m_unMvp = glGetUniformLocation(program, "mvp");
	if (-1 == m_unMvp)
	{
		std::cerr << "Failed to get uniform location: mvp\n";
		assert(false); return false;
	}

	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool Terrain::generateMeshData(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& colors)
{
	// Width and height of the heightmap.
	const int Width  = m_heightmap.getWidth();
	const int Height = m_heightmap.getHeight();

	assert(Width > 0 && Height > 0);

	// Generate vertex coordinates.

	// Vertex coordinates (3 per vertex).
	vertices.resize(Width * Height * 3);

	// Colors for height-based coloring.
	colors.resize(Width * Height);

	size_t currVertex = {};
	size_t currColor  = {};

	for (int z = 0; z < Height; ++z)
	{
		for (int x = 0; x < Width; ++x)
		{
			colors[currColor++] = m_heightmap.getActualHeightAtPoint(x, z) / 255.0f;

			const GLfloat ScaledHeight = m_heightmap.getScaledHeightAtPoint(x, z);

			vertices[currVertex++] = (GLfloat)x;
			vertices[currVertex++] = ScaledHeight;
			vertices[currVertex++] = (GLfloat)z;

			if (ScaledHeight < m_minHeightScaled)
			{
				m_minHeightScaled = ScaledHeight;
			}

			if (ScaledHeight > m_maxHeightScaled)
			{
				m_maxHeightScaled = ScaledHeight;
			}
		}
	}

	// Generate vertex indices.

	// Step 1. Generate consecutive integers to pick our indices from.

	std::unique_ptr< std::unique_ptr<GLuint[]>[] > spRawIndices =
		std::make_unique< std::unique_ptr<GLuint[]>[] >(Height);

	GLuint currentIndex = 0;

	for (int row = 0; row < Height; ++row)
	{
		spRawIndices[row] = std::make_unique<GLuint[]>(Width);

		for (int col = 0; col < Width; ++col)
		{
			spRawIndices[row][col] = currentIndex++;
		}
	}

	// Step 2. Pick indices for vertex rows.
	// Each row of triangle strip uses two adjacent vertex rows (top to bottom), 
	// hence (height - 1) and  (width * 2) in the declaration below.
	// Besides, we need a degenerate triangle after each strip row to "stitch" the strip row with the next one;
	// that requires 2 indices after each row except the last one.

	indices.resize((Height - 1) * (Width * 2) + 2 * (Height - 2));

	int currIndex = 0;

	for (int row = 0; row < (Height - 1); ++row)
	{
		for (int column = 0; column < Width; ++column)    // pick indices for the triangle strip row
		{
			indices[currIndex++] = spRawIndices[row][column];
			indices[currIndex++] = spRawIndices[row + 1][column];
		}

		// Add degenerate triangle to "stitch" the strip row with the next one (excluding the last strip row):
		// add the last index of the row we've just created, then the first index of the next row.

		if (row < (Height - 2))
		{
			indices[currIndex] = indices[currIndex - 1];
			++currIndex;

			indices[currIndex++] = spRawIndices[row + 1][0];
		}
	}

	return true;
}

void Terrain::updateViewMatrices(const std::unique_ptr<Camera>& spCamera) const
{
	assert(m_spProgram);
	assert(-1 != m_unMvp);

	// TODO: turn on
#if 0
	assert(-1 != m_unNormal);
#endif

	glUseProgram(m_spProgram->getProgram());

	glm::mat4 mvp = spCamera->getModelViewProjectionMatrix();

	glUniformMatrix4fv(m_unMvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glm::mat4 modelView = spCamera->getModelViewMatrix();

	// WARNING: we are using the fact that there are no non-uniform scaling. If this will change, use the entire 4x4 matrix.
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));

	//Normal = mat3(transpose(inverse(model))) * normal;

	// TODO: turn on
#if 0
	glUniformMatrix4fv(m_unNormal, 1, GL_FALSE, glm::value_ptr(normalMatrix));
#endif

	glUseProgram(0);
}

void Terrain::render() const
{
	assert(m_spProgram);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(m_spProgram->getProgram());

	glBindVertexArray(m_vao);

#if 0
	for (int tile = 0; tile < TILE_COUNT; ++tile)
	{
		glActiveTexture(GL_TEXTURE0 + tile);
		glBindTexture(GL_TEXTURE_2D, m_tiledTextures[tile]->m_texture);
	}
#endif

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	// TODO: temp
#if 0
	glDrawElements(GL_TRIANGLE_STRIP, m_indexCount - 16000, GL_UNSIGNED_INT, 0);
#else
	glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
#endif

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#if 0
	for (int tile = 0; tile < TILE_COUNT; ++tile)
	{
		glActiveTexture(GL_TEXTURE0 + tile);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#endif

	glBindVertexArray(0);
	glUseProgram(0);
}
