#include "stdafx.h"
#include "BlueAmberTerrains.h"
#include "BlueAmberTerrainsDlg.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Scene::Scene(GLfloat terrainScaleFactor, HDC hDC, int clientWidth, int clientHeight, const CAtlString& heighmapFilePath)
	: m_hDC(hDC), m_clientWidth{}, m_clientHeight{}, m_terrain(terrainScaleFactor)
{
	if (!m_hDC)
	{
		assert(false); throw EXCEPTION(L"Window DC is NULL");
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.64f, 0.91f, 1.0f);    // light blue

	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 0.02f; /*3.5f;*/

	if (0 == clientHeight)    // prevent dividing by zero
	{
		clientHeight = 1;
	}

	GLfloat aspectRatio = clientWidth / (GLfloat)clientHeight;

	// NOTE: don't catch std::bad_alloc here
	m_spCamera = std::make_unique<Camera>(aspectRatio, CameraScaleFactor, FieldOfView, FrustumNear, FrustumFar);

#if 1
	// Get our future terrain in focus.
	m_spCamera->translateX(-0.8f);
	m_spCamera->translateY(-0.8f);
	m_spCamera->translateZ(-1.5f);
#endif

	// Load heightmap and initialize terrain.
	if (!m_terrain.loadHeightmapFromFile(heighmapFilePath))
	{
		assert(false); throw EXCEPTION_FMT(L"Failed to load heightmap: %s", heighmapFilePath);
	}
	
	SlopeLightingParams slopeLighting;
	slopeLighting.m_lightDirectionX = 1;
	slopeLighting.m_lightDirectionZ = 1;
	slopeLighting.m_minBrightness   = 0.2f;
	slopeLighting.m_maxBrightness   = 0.9f;
	slopeLighting.m_lightSoftness   = 17.0f;
	
	if (!m_terrain.initialize(slopeLighting))
	{
		assert(false); throw EXCEPTION(L"Terrain initialization failed");
	}
}

Scene::~Scene()
{
}

void Scene::resize(int clientWidth, int clientHeight)
{
	if (0 == clientHeight)    // prevent dividing by zero
	{
		clientHeight = 1;
	}

	// Resize the viewport.
	glViewport(0, 0, clientWidth, clientHeight);

	// Calculate aspect ratio of the window.
	gluPerspective(FieldOfView, clientWidth / (GLfloat)clientHeight, FrustumNear, FrustumFar);
}

void Scene::translateCameraX(GLfloat diff)
{
	m_spCamera->translateX(diff);
}

void Scene::translateCameraY(GLfloat diff)
{
	m_spCamera->translateY(diff);
}

void Scene::translateCameraZ(GLfloat diff)
{
	m_spCamera->translateZ(diff);
}

void Scene::rotateCameraX(GLfloat angle)
{
	m_spCamera->rotateX(angle);
}

void Scene::rotateCameraY(GLfloat angle)
{
	m_spCamera->rotateY(angle);
}

void Scene::rotateCameraZ(GLfloat angle)
{
	m_spCamera->rotateZ(angle);
}

void Scene::scaleCamera(GLfloat amount)
{
	m_spCamera->scale(amount);
}

void Scene::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // wireframe mode

#if 1
	m_terrain.updateViewMatrices(m_spCamera);
	m_terrain.render();
#endif

	SwapBuffers(m_hDC);
}
