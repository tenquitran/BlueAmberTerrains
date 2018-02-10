// BlueAmberTerrainsDlg.cpp : implementation file
#include "stdafx.h"
#include "BlueAmberTerrains.h"
#include "BlueAmberTerrainsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

using namespace BlueAmberTerrainsApp;

//////////////////////////////////////////////////////////////////////////


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CBlueAmberTerrainsDlg dialog
//////////////////////////////////////////////////////////////////////////


CBlueAmberTerrainsDlg::CBlueAmberTerrainsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBlueAmberTerrainsDlg::IDD, pParent), m_hRC(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlueAmberTerrainsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBlueAmberTerrainsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HEIGHTMAP_OPEN, &OnOpenHeightmap)
	ON_COMMAND(ID_MAIN_EXIT, &OnExitApplication)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CBlueAmberTerrainsDlg message handlers

BOOL CBlueAmberTerrainsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set up OpenGL context for our window.

	const int OpenGlMajor = 4;
	const int OpenGlMinor = 4;

	if (!setupOpenGlContext(OpenGlMajor, OpenGlMinor))
	{
		std::cerr << "Failed to set up OpenGL context (version " << OpenGlMajor << "." << OpenGlMinor << ")\n";
		assert(false); return TRUE;
	}

	// Create and initialize the scene.

	m_spScene = std::make_unique<Scene>(0.25f, GetDC()->m_hDC);

	int clientWidth  = {};
	int clientHeight = {};

	getClientSize(clientWidth, clientHeight);

	if (!m_spScene->initialize(clientWidth, clientHeight))
	{
		std::cerr << "Scene initialization failed\n";
		assert(false); return TRUE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CBlueAmberTerrainsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
#if 0
		CDialogEx::OnPaint();
#else
		if (m_spScene)
		{
			m_spScene->render();
		}
#endif
	}
}

// The system calls this function to obtain the cursor to display while the user drags the minimized window.
HCURSOR CBlueAmberTerrainsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CBlueAmberTerrainsDlg::setupOpenGlContext(int versionMajor, int versionMinor)
{
	if (!m_hWnd)
	{
		std::cerr << __FUNCTION__ << ": window handle is NULL\n";
		assert(false); return false;
	}
	else if (versionMajor < 1)
	{
		std::cerr << __FUNCTION__ << ": invalid OpenGL major version: " << versionMajor << '\n';
		assert(false); return false;
	}

	HDC hDC = GetDC()->m_hDC;
	if (!hDC)
	{
		std::cerr << __FUNCTION__ << ": device context is NULL\n";
		assert(false); return false;
	}

	// Step 1. Set pixel format for the Windows DC.

	PIXELFORMATDESCRIPTOR pfd = {};

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_SUPPORT_OPENGL |    // OpenGL window
		PFD_DRAW_TO_WINDOW |    // render to window
		PFD_DOUBLEBUFFER;       // support double-buffering
	pfd.iPixelType = PFD_TYPE_RGBA;       // color type: red, green, blue, and alpha
	pfd.cColorBits = 32;                  // preferred color depth (bits per pixel for each color buffer): 8, 16, 24, or 32
	pfd.cDepthBits = 24;                  // depth of the depth (z-axis) buffer
	//pfd.iLayerType = PFD_MAIN_PLANE;    // main layer. Ignored in OpenGL 3.0 and later

	// The OS/driver will try to find the matching pixel format.
	// If some value cannot be set, it will be replaced by the highest possible value (e.g. 24-bit colors instead of 32-bit).
	// Returns an integer ID of the pixel format.
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (0 == pixelFormat)
	{
		std::cerr << __FUNCTION__ << ": ChoosePixelFormat() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Set the pixel format for the device context and the associated window.
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		std::cerr << __FUNCTION__ << ": SetPixelFormat() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 2. Create a temporary OpenGL rendering context to try to get the latest one - see below.
	HGLRC hRcTmp = wglCreateContext(hDC);
	if (!hRcTmp)
	{
		std::cerr << __FUNCTION__ << ": wglCreateContext() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 3. Make the temporary rendering context current for our thread.
	if (!wglMakeCurrent(hDC, hRcTmp))
	{
		std::cerr << __FUNCTION__ << ": wglMakeCurrent() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 4. Initialize GLEW (in particular, to be able to conveniently use the wglCreateContextAttribsARB extension.

	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << __FUNCTION__ << "glewInit() failed: " << (char *)glewGetErrorString(err) << '\n';
		assert(false); return false;
	}

	// Step 5. Set up the modern OpenGL rendering context.

	// Set the OpenGL version required.
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
		WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,

#if _DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#endif

		0 };    // zero indicates the end of the array

	// If the pointer to this extension is NULL, the OpenGL version is not supported.
	if (!wglCreateContextAttribsARB)
	{
		std::cerr << __FUNCTION__ << ": OpenGL version " << versionMajor << "." << versionMinor << " not supported\n";
		assert(false); return false;
	}

	// Create a modern OpenGL context.
	m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if (!m_hRC)
	{
		std::cerr << __FUNCTION__ << "wglCreateContextAttribsARB() failed\n";
		assert(false); return false;
	}

	// On success, delete the temporary context.
	wglDeleteContext(hRcTmp);

	// Step 6. Make the final rendering context current for our thread.
	if (!wglMakeCurrent(hDC, m_hRC))
	{
		std::cerr << __FUNCTION__ << ": wglMakeCurrent() failed (2): " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	glDebugMessageCallback(openGlDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	return true;
}

BOOL CBlueAmberTerrainsDlg::ContinueModal()
{
	BOOL res = CDialogEx::ContinueModal();

	if (!res)
	{   // The main window will be closed. Do the cleanup.

		if (m_hRC)
		{
			// Deselect the rendering context AND release the DC used by the rendering context.
			if (!wglMakeCurrent(nullptr, nullptr))
			{
				std::cerr << "wglMakeCurrent() failed on exit: " << ::GetLastError() << '\n';
				assert(false);
			}

			// Delete the rendering context.
			if (!wglDeleteContext(m_hRC))
			{
				std::cerr << "wglDeleteContext() failed on exit: " << ::GetLastError() << '\n';
				assert(false);
			}

			m_hRC = nullptr;
		}

		// WARNING: although MSDN states that deselecting the rendering context with wglMakeCurrent(NULL, NULL) 
		// causes releasing of the device context used by the rendering context, 
		// this probably means the rendering context is no longer coupled to that particular device context, 
		// NOT that the device context is released. Anyway, the code below works without errors.

		CDC *pDC = GetDC();

		if (   pDC
			&& !ReleaseDC(pDC))
		{
			std::cerr << "ReleaseDC() failed on exit: " << ::GetLastError() << '\n';
			assert(false);
		}

	}   // The main window will be closed. Do the cleanup.

	return res;
}

void APIENTRY CBlueAmberTerrainsDlg::openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* param)
{
	UNREFERENCED_PARAMETER(length);
	UNREFERENCED_PARAMETER(param);

	std::cout << "Debug message from the ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "OpenGL API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "window system";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "shader compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "third party tools or libraries";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "application (explicit)";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "other source";
		break;
	default:    // unknown source?
		assert(false); break;
	}

	std::cout << "\nMessage text: " << message;

	std::cout << "\nType: ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}

	std::cout << "\nID: " << id;

	std::cout << "\nSeverity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	}
	std::cout << std::endl;
}

void CBlueAmberTerrainsDlg::OnOpenHeightmap()
{
	CFileDialog dlg(TRUE, L"png", L"*.png");
	// TODO: temp
	dlg.GetOFN().lpstrInitialDir = L"D:\\natProgs\\graphics2\\BlueAmberTerrains\\BlueAmberTerrains\\data";

	if (IDOK == dlg.DoModal())
	{
		CFile file;
		CAtlString filePath;

		if (file.Open(filePath = dlg.GetPathName(), CFile::modeRead))
		{
			if (!loadHeightmapFromFile(filePath))
			{
				std::cerr << "Failed to load heightmap: " << std::wstring_convert< std::codecvt_utf8<wchar_t> >().to_bytes(filePath) << '\n';
			}
		}
	}
}

bool CBlueAmberTerrainsDlg::loadHeightmapFromFile(const CAtlString& filePath)
{
	if (!m_spScene)
	{
		assert(false); return false;
	}

	return m_spScene->loadHeightmapFromFile(filePath);
}

void CBlueAmberTerrainsDlg::OnExitApplication()
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CBlueAmberTerrainsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (!m_hRC)
	{
		return;    // the OpenGL context is not yet created
	}

	int clientWidth  = {};
	int clientHeight = {};

	getClientSize(clientWidth, clientHeight);

	m_spScene->resize(clientWidth, clientHeight);
}

void CBlueAmberTerrainsDlg::getClientSize(int& clientWidth, int& clientHeight)
{
	RECT rect;
	GetClientRect(&rect);

	clientWidth  = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
}

void CBlueAmberTerrainsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

UINT CBlueAmberTerrainsDlg::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;    // allow processing of the arrow keys, etc.
}

void CBlueAmberTerrainsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: up/down scene translation - ?

	switch (nChar)
	{
	case VK_ESCAPE:    // exit the application
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		break;
	case VK_SHIFT:    // Shift key is released: consider the mouse movement (if any) ended
		{
			POINT pt;

			if (   ::GetCursorPos(&pt)
				&& ::ScreenToClient(m_hWnd, &pt))
			{
				m_mouseMovement.onShiftPressed(pt);
			}
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	// Translate the camera.
	case 0x57:    // W key: move the scene forward
		m_spScene->translateCameraZ(0.5f);
		//m_spScene->translateCameraY(0.5f);
		break;
	case 0x53:    // S key: move the scene backward
		m_spScene->translateCameraZ(-0.5f);
		//m_spScene->translateCameraY(-0.5f);
		break;
	case 0x41:    // A key: move the scene left
		m_spScene->translateCameraX(-0.5f);
		break;
	case 0x44:    // D key: move the scene right
		m_spScene->translateCameraX(0.5f);
		break;
	//////////////////////////////////////////////////////////////////////////
	// Zoom the camera in/out.
	case VK_NUMPAD1:    // zoom in
		m_spScene->scaleCamera(0.05f);
		break;
	case VK_NUMPAD2:    // zoom out
		m_spScene->scaleCamera(-0.05f);
		break;
	//////////////////////////////////////////////////////////////////////////
	// Rotate the camera.
	case VK_UP:
		m_spScene->rotateCameraX(2.0f);
		break;
	case VK_DOWN:
		m_spScene->rotateCameraX(-2.0f);
		break;
	case VK_LEFT:
		m_spScene->rotateCameraY(-2.0f);
		break;
	case VK_RIGHT:
		m_spScene->rotateCameraY(2.0f);
		break;
	default:
		break;
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBlueAmberTerrainsDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Shift key is released: consider the mouse movement (if any) ended
	if (VK_SHIFT == nChar)
	{
		m_mouseMovement.onShiftReleased();
	}

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CBlueAmberTerrainsDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)    // forward rotation: zoom in
	{
		m_spScene->scaleCamera(0.05f);
	} 
	else               // backward rotation: zoom out
	{
		m_spScene->scaleCamera(-0.05f);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CBlueAmberTerrainsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
#if 0//_DEBUG
	CAtlString msg;
	msg.Format(L"Mouse: %ld, %ld\n", point.x, point.y);
	::OutputDebugStringW(msg);
#endif

	//if (MK_MBUTTON == nFlags)    // mouse will control the camera only while its middle button is pressed
	if (MK_SHIFT == nFlags)    // mouse will control the camera only while the Shift key is pressed
	{
		LONG offsetX = {};
		LONG offsetY = {};

		if (m_mouseMovement.getOffset(point, offsetX, offsetY))
		{
			// TODO: control the camera
#if _DEBUG
			CAtlString msg;
			msg.Format(L"Mouse offset: %ld, %ld\n", offsetX, offsetY);
			::OutputDebugStringW(msg);
#endif
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
