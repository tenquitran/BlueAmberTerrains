#include "stdafx.h"
#include "BlueAmberTerrains.h"
#include "BlueAmberTerrainsDlg.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Scene::Scene(GLfloat terrainScaleFactor, HDC hDC)
	: m_hDC(hDC), m_clientWidth{}, m_clientHeight{}, m_terrain(terrainScaleFactor)
{
	if (!m_hDC)
	{
		assert(false); throw EXCEPTION(L"Window DC is NULL");
	}
}

Scene::~Scene()
{
}

bool Scene::initialize(int clientWidth, int clientHeight)
{
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
	// Get our terrain in focus.
	m_spCamera->translateX(-0.8f);
	m_spCamera->translateY(-0.8f);
	m_spCamera->translateZ(-1.5f);

	//m_spCamera->translateX(-55.0f);
#endif

	try
	{
		// TODO: temp - load the hard-coded heightmap.
		CAtlString filePath = L"D:\\natProgs\\graphics2\\BlueAmberTerrains\\BlueAmberTerrains\\data\\heightmap1.png";
		if (!loadHeightmapFromFile(filePath))
		{
			std::cerr << "Failed to load heightmap: " << std::wstring_convert< std::codecvt_utf8<wchar_t> >().to_bytes(filePath) << '\n';
			return false;
		}

		if (!m_terrain.initialize())
		{
			std::cerr << "Terrain initialization failed\n";
			return false;
		}
	}
	catch (const Exception& ex)
	{
		std::cerr << "Failed to initialize scene: " << std::wstring_convert< std::codecvt_utf8<wchar_t> >().to_bytes(ex.message()) << '\n';
		assert(false);
	}
	catch (const std::bad_alloc&)
	{
		std::cerr << "Memalloc failure while initializing the scene\n";
		assert(false);
	}

	return true;
}

bool Scene::loadHeightmapFromFile(const CAtlString& filePath)
{
	return m_terrain.loadHeightmapFromFile(filePath);
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
	//gluPerspective(45.0f, m_clientWndWidth / (GLfloat)m_clientWndHeight, 0.1, 1000.0f);
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
