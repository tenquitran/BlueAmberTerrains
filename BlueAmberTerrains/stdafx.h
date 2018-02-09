// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Additional headers our program requires.
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <memory>
#include <vector>
#include <map>

// GLEW library (http://glew.sourceforge.net/)
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32.lib")

// GLM library (http://glm.g-truc.net/)
#define GLM_FORCE_RADIANS    // enforce all angles to be in radians, not in degrees
#include <glm/glm.hpp>       // the most common GLM headers
#include <glm/gtc/matrix_transform.hpp>    // matrix transformations: glm::perspective(), etc.
#include <glm/gtc/type_ptr.hpp>            // glm::value_ptr() to pass matrices to shaders, etc.

// WGL (Windows OpenGL)
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

// SOIL (Simple OpenGL Image Loader) library (http://www.lonesock.net/soil.html)
#include <soil/src/SOIL.h>
#pragma comment(lib, "SOIL.lib")


namespace BlueAmberTerrainsApp
{
	// For OpenGL code.
#define BUFFER_OFFSET(offset) ((void *)(offset))
}


#include "Exception.h"

// Common OpenGL functionality.
#include "ShaderHelpers.h"
#include "ProgramGLSL.h"
#include "Camera.h"

#include "HeightData.h"
#include "Heightmap.h"
#include "Terrain.h"
#include "Scene.h"
