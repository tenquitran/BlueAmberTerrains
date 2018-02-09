#pragma once


namespace BlueAmberTerrainsApp
{
	class Scene
	{
	public:
		explicit Scene(GLfloat terrainScaleFactor);

		virtual ~Scene();

		bool initialize();

		// Load heightmap data from the file specified.
		// Parameters: filePath - full path to the file.
		bool loadHeightmapFromFile(const CAtlString& filePath);

	private:
		// Get aspect ratio of the main window's client area.
		GLfloat getMainWindowAspectRatio() const;

	private:
		std::unique_ptr<Camera> m_spCamera;

		Terrain m_terrain;
	};
}
