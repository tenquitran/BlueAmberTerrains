// BlueAmberTerrains.h : main header file for the PROJECT_NAME application
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class CBlueAmberTerrainsApp 
	: public CWinApp
{
public:
	CBlueAmberTerrainsApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	// Load heightmap data from the file specified.
	// Parameters: filePath - full path to the file.
	bool loadHeightmapFromFile(const CAtlString& filePath);

private:
	std::unique_ptr<BlueAmberTerrainsApp::Scene> m_spScene;
};

extern CBlueAmberTerrainsApp theApp;