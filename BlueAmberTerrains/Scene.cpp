#include "stdafx.h"
#include "BlueAmberTerrains.h"
#include "BlueAmberTerrainsDlg.h"

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


Scene::Scene(GLfloat terrainScaleFactor)
	: m_terrain(terrainScaleFactor)
{
}

Scene::~Scene()
{
}

bool Scene::initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 0.02f; /*3.5f;*/

	// Field of view angle.
	const GLfloat FieldOfView = 45.0f;

	// Frustum boundaries.
	const GLfloat FrustumNear = 0.1f;
	const GLfloat FrustumFar  = 1000.0f;

	const GLfloat AspectRatio = getMainWindowAspectRatio();

	// NOTE: don't catch std::bad_alloc here
	m_spCamera = std::make_unique<Camera>(AspectRatio, CameraScaleFactor, FieldOfView, FrustumNear, FrustumFar);

#if 1
	// Get our terrain in focus.
	m_spCamera->translateX(-0.8f);
	m_spCamera->translateY(-0.8f);
	m_spCamera->translateZ(-1.5f);

	//m_spCamera->translateX(-55.0f);
#endif

	try
	{
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

GLfloat Scene::getMainWindowAspectRatio() const
{
	CBlueAmberTerrainsDlg *pMainDlg = (CBlueAmberTerrainsDlg *)AfxGetMainWnd();
	if (!pMainDlg)
	{
		std::cerr << "Scene initialization: no main window\n";
		assert(false); return false;
	}

	RECT clientRect = {};
	pMainDlg->GetClientRect(&clientRect);

	return (clientRect.right - clientRect.left) / (float)(clientRect.bottom - clientRect.top);
}

bool Scene::loadHeightmapFromFile(const CAtlString& filePath)
{
	return m_terrain.loadHeightmapFromFile(filePath);
}
