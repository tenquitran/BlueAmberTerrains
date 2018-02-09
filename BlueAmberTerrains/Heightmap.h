#pragma once


namespace BlueAmberTerrainsApp
{
	class Heightmap
	{
	public:
		// Parameters: scaleFactor - scale factor of the heightmap.
		explicit Heightmap(GLfloat scaleFactor);

		virtual ~Heightmap();

		bool isEmpty() const;

		int getWidth() const;
		int getHeight() const;

		// Get actual height at the specified point.
		// Throws: Exception
		unsigned char getActualHeightAtPoint(int x, int z) const;

		// Get scaled height at the specified point.
		// Throws: Exception
		GLfloat getScaledHeightAtPoint(int x, int z) const;

		// Load heightmap data from the file specified.
		// Parameters: filePath - full path to the file.
		bool loadFromFile(const CAtlString& filePath);

	private:
		Heightmap(const Heightmap&) = delete;
		Heightmap& operator=(const Heightmap&) = delete;

	private:
		HeightData m_heightData;
	};
}
