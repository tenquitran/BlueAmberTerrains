#pragma once


namespace BlueAmberTerrainsApp
{
	class Terrain
	{
	public:
		// Parameters: scaleFactor - scale factor of the heightmap.
		explicit Terrain(GLfloat scaleFactor);

		virtual ~Terrain();

		// Load heightmap data from the file specified.
		// Parameters: filePath - full path to the file.
		bool loadHeightmapFromFile(const CAtlString& filePath);

		// Throws: std::bad_alloc
		bool initialize();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		void updateViewMatrices(const std::unique_ptr<Camera>& spCamera) const;

		void render() const;

	private:
		// Generate data for the terrain mesh.
		// Parameters: vertices - vertex coordinates;
		//             indices - vertex indices;
		//             colors - colors for height-based coloring.
		// Returns: true on success, false otherwise.
		// Throws: std::bad_alloc
		bool generateMeshData(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<GLfloat>& colors);

	private:
		// Terrain heightmap.
		Heightmap m_heightmap;

		std::unique_ptr<ProgramGLSL> m_spProgram;    // GLSL program wrapper

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLint m_unMvp;     // MVP matrix uniform

		GLuint m_color;    // buffer for height-based vertex colors

		// Minimum and maximum scaled height of the terrain.
		GLfloat m_minHeightScaled;
		GLfloat m_maxHeightScaled;
	};
}
