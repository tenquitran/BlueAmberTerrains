// BlueAmberTerrainsDlg.h : header file
#pragma once


// Main dialog window.
class CBlueAmberTerrainsDlg 
	: public CDialogEx
{
	// Mouse movement data to control the camera.
	struct MouseMovement 
	{
	public:
		MouseMovement()
			: m_previous{}, m_shiftPressed(false)
		{
		}

		void onShiftPressed(const POINT& ptCurrent)
		{
			if (!m_shiftPressed)    // toggle only
			{
				m_shiftPressed = true;

#if _DEBUG
				::OutputDebugStringW(L"Shift pressed\n");
#endif

				m_previous = ptCurrent;
			}
		}

		void onShiftReleased()
		{
			m_shiftPressed = false;

#if _DEBUG
			::OutputDebugStringW(L"Shift released\n");
#endif

			m_previous = POINT{};
		}

		// Calculate offset between the new and the previous mouse coordinates.
		bool getOffset(const POINT& ptNew, LONG& offsetX, LONG& offsetY)
		{
			if (!m_shiftPressed)
			{
				return false;
			}

			if (   0 == m_previous.x
				&& 0 == m_previous.y)
			{
				m_previous = ptNew;
				return false;
			} 

			offsetX = ptNew.x - m_previous.x;
			offsetY = ptNew.y - m_previous.y;
			return true;
		}

	public:
		// Previous position of the mouse cursor (in client coordinates).
		// Stored only while the Shift key is pressed.
		POINT m_previous;

		// true if the Shift key is currently pressed.
		bool m_shiftPressed;
	};

public:
	CBlueAmberTerrainsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BLUEAMBERTERRAINS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Perform cleanup on closing the window.
	virtual BOOL ContinueModal() override;

private:
	// Get width and height of the main window's client area.
	void getClientSize(int& clientWidth, int& clientHeight);

	// Create and set up OpenGL context for the window.
	// Parameters: versionMajor - major version number of OpenGL;
	//             versionMinor - minor version number of OpenGL.
	bool setupOpenGlContext(int versionMajor, int versionMinor);

	// Debug callback for OpenGL-related errors.
	static void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* param);

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// Open existing heightmap file.
	afx_msg void OnOpenHeightmap();

	afx_msg void OnExitApplication();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

private:
	// OpenGL rendering context.
	HGLRC m_hRC;

	// Terrain scene.
	std::unique_ptr<BlueAmberTerrainsApp::Scene> m_spScene;

	MouseMovement m_mouseMovement;
};
