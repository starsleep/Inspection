#pragma once
#include "DrawWnd.h"


class CameraThread : public CWinThread
{
	DECLARE_DYNCREATE(CameraThread)
	
	//friend InspectionSystem;

protected:
	
private :

	BOOL				m_bThUse; //Run Thread Á¾·á

	CRITICAL_SECTION	m_csCam;

public :

	virtual BOOL InitInstance();

	virtual int Exitlnstance();

	virtual int Run();

	void SetThreadState(BOOL use);
};

