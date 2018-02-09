#pragma once


namespace BlueAmberTerrainsApp
{
	// Heightmap data.
	struct HeightData
	{
	public:
		// Parameters: scaleFactor - scale factor of the heightmap.
		explicit HeightData(GLfloat scaleFactor);

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

		virtual ~HeightData();

	private:
		HeightData(const HeightData&) = delete;
		HeightData& operator=(const HeightData&) = delete;

		// Get actual height of the specified element.
		// Throws: Exception
		unsigned char operator[](int index) const;

	private:
		// Scale factor of the height data.
		const GLfloat ScaleFactor;

		// Heightmap data.
		unsigned char *m_pData;

		// Number of elements of the heightmap data.
		int m_size;

		// Width and height of the heightmap data file.
		int m_width;
		int m_height;
	};
}
