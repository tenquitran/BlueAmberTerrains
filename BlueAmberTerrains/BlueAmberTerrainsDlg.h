// BlueAmberTerrainsDlg.h : header file
#pragma once


// CBlueAmberTerrainsDlg dialog
class CBlueAmberTerrainsDlg 
	: public CDialogEx
{
// Construction
public:
	CBlueAmberTerrainsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BLUEAMBERTERRAINS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// Open existing heightmap file.
	afx_msg void OnOpenHeightmap();

	DECLARE_MESSAGE_MAP()
};
