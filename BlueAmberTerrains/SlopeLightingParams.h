#pragma once


namespace BlueAmberTerrainsApp
{
	// Parameters to calculate slope lighting.
	struct SlopeLightingParams
	{
	public:
		SlopeLightingParams()
			: m_lightDirectionX{}, m_lightDirectionZ{}, m_minBrightness{}, m_maxBrightness{}, m_lightSoftness{}
		{
		}

	public:
		int m_lightDirectionX;
		int m_lightDirectionZ;

		GLfloat m_minBrightness;
		GLfloat m_maxBrightness;

		GLfloat m_lightSoftness;
	};
}
