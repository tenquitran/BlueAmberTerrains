#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


HeightData::HeightData(GLfloat scaleFactor)
	: ScaleFactor(scaleFactor), m_pData(nullptr), m_size{}, m_width{}, m_height{}
{
}

HeightData::~HeightData()
{
	if (m_pData)
	{
		SOIL_free_image_data(m_pData);
	}
}

bool HeightData::isEmpty() const
{
	return (nullptr == m_pData);
}

int HeightData::getWidth() const
{
	return m_width;
}

int HeightData::getHeight() const
{
	return m_height;
}

bool HeightData::loadFromFile(const CAtlString& filePath)
{
	// Release the previous heightmap data.
	if (m_pData)
	{
		SOIL_free_image_data(m_pData);
		m_pData = nullptr;

		m_width  = {};
		m_height = {};
		m_size   = {};
	}

	const CAtlStringA filePathA = CW2A(filePath);

	// The SOIL_LOAD_L flag loads the heightmap as a greyscale image.
	m_pData = SOIL_load_image(filePathA.GetString(), &m_width, &m_height, 0, SOIL_LOAD_L);
	if (!m_pData)
	{
		std::cerr << "Failed to load heightmap \"" << filePathA << "\": " << SOIL_last_result() << '\n';
		assert(false); return false;
	}

	m_size = m_width * m_height;

	return true;
}

unsigned char HeightData::getActualHeightAtPoint(int x, int z) const
{
	return (*this)[z * m_width + x];
}

GLfloat HeightData::getScaledHeightAtPoint(int x, int z) const
{
	return (ScaleFactor * (GLfloat)(*this)[z * m_width + x]);
}

unsigned char HeightData::operator[](int index) const
{
	if (   index < 0
		|| index >= m_size)
	{
		throw EXCEPTION_FMT(L"Index %d out of bounds", index);
	}

	return m_pData[index];
}
