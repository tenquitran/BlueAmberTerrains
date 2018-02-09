#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Heightmap::Heightmap(GLfloat scaleFactor)
	: m_heightData(scaleFactor)
{
}

Heightmap::~Heightmap()
{
}

bool Heightmap::isEmpty() const
{
	return m_heightData.isEmpty();
}

int Heightmap::getWidth() const
{
	return m_heightData.getWidth();
}

int Heightmap::getHeight() const
{
	return m_heightData.getHeight();
}

bool Heightmap::loadFromFile(const CAtlString& filePath)
{
	return m_heightData.loadFromFile(filePath);
}

unsigned char Heightmap::getActualHeightAtPoint(int x, int z) const
{
	return m_heightData.getActualHeightAtPoint(x, z);
}

GLfloat Heightmap::getScaledHeightAtPoint(int x, int z) const
{
	return m_heightData.getScaledHeightAtPoint(x, z);
}
