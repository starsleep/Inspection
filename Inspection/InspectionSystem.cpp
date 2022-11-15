#include "pch.h"
#include "InspectionSystem.h"

using namespace std;
using namespace cv;

void InspectionSystem::initialization(std::vector<void*>& vpptr)
{
	//dynamic allocation//
	
	m_bLive = FALSE;

	if (m_pVideoCap != nullptr) { SAFE_DELETE(m_pVideoCap);}
	m_pVideoCap = new VideoCapture;

	if (m_pFrame != nullptr) { SAFE_DELETE(m_pFrame); }
	m_pFrame = new Mat;

	if (m_pDisplay != nullptr) { SAFE_DELETE(m_pDisplay); }
	m_pDisplay = new DrawWnd;

	//Thread
	if (m_pCamThread != nullptr) { SAFE_DELETE(m_pCamThread); }

	m_pCamThread = static_cast<CameraThread*>(AfxBeginThread(RUNTIME_CLASS(CameraThread),THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED));
	m_pInspThread = static_cast<InspectionThread*>(AfxBeginThread(RUNTIME_CLASS(InspectionThread), THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED));
	int index = 0;
	m_pDlg = reinterpret_cast<CWnd*>(vpptr[index]); index++;
	m_pMainDisplay = reinterpret_cast<CWnd*>(vpptr[index]); //index++;
	m_pDisplay->initialization(m_pMainDisplay);
	vpptr.clear();
}

void InspectionSystem::ExitSystem()
{
	int CamThreaderr = m_pCamThread->Exitlnstance();
	int InspThreaderr = m_pInspThread->Exitlnstance();

	if(CamThreaderr == 0 && InspThreaderr == 0)
	{
		if (m_bLive == TRUE)
		{
			::WaitForSingleObject(m_eCamQuit, INFINITE);
			::WaitForSingleObject(m_eInspQuit, INFINITE);
		}
		m_pCamThread = nullptr;
		m_pInspThread = nullptr;

	}

	if (m_pVideoCap != nullptr) { SAFE_DELETE(m_pVideoCap); }


	if (m_pFrame != nullptr) { SAFE_DELETE(m_pFrame); }


	if (m_pDisplay != nullptr) { SAFE_DELETE(m_pDisplay); }
}

BOOL InspectionSystem::OpenCamera(int CamIndex)
{
	m_pVideoCap->open(CamIndex);
	if (!m_pVideoCap->isOpened()) 
	{
		return FALSE;
	}
	else
	{
		m_pVideoCap->set(CAP_PROP_FRAME_WIDTH, 3264);
		m_pVideoCap->set(CAP_PROP_FRAME_HEIGHT, 2448);
		return TRUE;
	}
}

void InspectionSystem::SetIamgeType(ImageType eIamge)
{
	//Get IamgeType ex) color , gary ...
	m_eImageType = eIamge;
}

void InspectionSystem::LIVE()
{
	//start Thread
	m_pCamThread->SetThreadState(TRUE);
	m_pCamThread->ResumeThread();
	m_bLiveState = TRUE;
	m_bLive = TRUE;
}

void InspectionSystem::LIVEFit()
{
	if (m_pFrame->empty()) { return; }

	m_pDisplay->SetRetore();
	if (m_bLive == FALSE)
	{
		m_pDisplay->Display();
	}
}

void InspectionSystem::ZoomIn()
{
	if (m_pFrame->empty()) { return; }

	m_pDisplay->Zoom(1, 0);
	if (m_bLive == FALSE) 
	{
		m_pDisplay->Display();
	}
}

void InspectionSystem::StopCamThread()
{
	m_pCamThread->SuspendThread();
	m_bLive = FALSE;
}

void InspectionSystem::StopInspetionThread()
{
	m_pInspThread->SuspendThread();
	m_bLive = FALSE;
}

void InspectionSystem::ZoomOut()
{
	if (m_pFrame->empty()) { return; }

	m_pDisplay->Zoom(-1, 0);
	if (m_bLive == FALSE)
	{
		m_pDisplay->Display();
	}
}

void InspectionSystem::move(CPoint cpBack, CPoint cpnow)
{
	if (m_pFrame->empty()) { return; }

	m_pDisplay->move(cpBack, cpnow);
	if (m_bLive == FALSE)
	{
		m_pDisplay->Display();
	}
}

InspData InspectionSystem::ReadRecipe()
{
	setlocale(LC_ALL, "");

	CString Path, str , m_savedata[11];
	Path.Format(L"%s\\Recipe.txt", INSPECTION_PATH);
	BOOL bOpen = m_file.Open(Path, CStdioFile::modeRead | CFile::typeText);
	if (bOpen == FALSE)
	{
		AfxMessageBox(_T("Open Error"));
	}
	for (int i = 0; i < 11; i++)
	{
		m_file.ReadString(m_savedata[i]);
	}
	int index = 0;
	int pos = 0;

	m_InspData.topX = _ttoi(m_savedata[index]); index++;

	m_InspData.topY = _ttoi(m_savedata[index]); index++;
	
	m_InspData.BotX = _ttoi(m_savedata[index]); index++;
	
	m_InspData.BotY = _ttoi(m_savedata[index]); index++;
	
	m_InspData.ThresHold = _ttoi(m_savedata[index]); index++;
	
	m_InspData.EdgeHigh = _ttoi(m_savedata[index]); index++;
	
	m_InspData.EdgeLOW = _ttoi(m_savedata[index]); index++;
	
	m_InspData.Standard = _ttoi(m_savedata[index]); index++;
	
	m_InspData.Upper = _ttoi(m_savedata[index]); index++;
	
	m_InspData.Lower = _ttoi(m_savedata[index]); index++;

	m_InspData.eImageType = _ttoi(m_savedata[index]);

	m_file.Close();

	return m_InspData;
}

void InspectionSystem::StartInspetion()
{
	m_pInspThread->SetThreadState(TRUE);
	m_pInspThread->ResumeThread();
	m_bLiveState = FALSE;
}

void InspectionSystem::ImageSave()
{
	if (m_pFrame->empty()) 
	{
		AfxMessageBox(L"이미지가 비어있습니다.");
		return;
	}
	CString str;
	str.Format(L"%s\\ImageSave.bmp", IMAGESAVE_PATH);
	String savePath = std::string(CT2CA(str));
	imwrite((cv::String)savePath, *m_pFrame);
	//imwrite("C:\\INSPECTION\\IMAGESAVE.bmp", save);
	AfxMessageBox(L"이미지가 저장되었습니다.");
}
