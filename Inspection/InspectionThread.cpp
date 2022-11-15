#include "pch.h"
#include "InspectionSystem.h"
#include "InspectionThread.h"


using namespace cv;

IMPLEMENT_DYNCREATE(InspectionThread, CWinThread);

InspectionSystem* m_pSys;

BOOL InspectionThread::InitInstance()
{
	m_pSys = InspectionSystem::GetInstance();
	::InitializeCriticalSection(&m_csInsp);

	return TRUE;
}

int InspectionThread::Exitlnstance()
{
	m_bThUse = FALSE;
	return 0;
}

int InspectionThread::Run()
{
	DrawWnd* pDraw = m_pSys->GetDrawWnd();
	cv::Mat* pImage = m_pSys->GetMat();
	cv::VideoCapture* pVideoCap = m_pSys->GetVideoCapture();

	if (pDraw == nullptr || pImage == nullptr || pVideoCap == nullptr)
	{
		m_pSys->m_eInspQuit.SetEvent();
		return -1;
	}

	ReSetCount();

	while(m_bThUse)
	{
		::WaitForSingleObject(m_pSys->m_InspEvent, INFINITE);
		m_pSys->m_InspEvent.ResetEvent();
		
		::EnterCriticalSection(&m_csInsp);
		
		if (pDraw == nullptr || pImage == nullptr || pVideoCap == nullptr)
		{
			::LeaveCriticalSection(&m_csInsp); return -1;
		}
		m_InspData = m_pSys->GetInspdata();

		BOOL bTem = GetTemplate(m_Template);

		if (bTem == FALSE) 
		{
			::LeaveCriticalSection(&m_csInsp); return -1;
		}

		pDraw->ChangeImageType((*pImage), m_pSys->GetIamgeType(), m_InspData);		//영상 전처리
		pDraw->ChangeImageType(m_Template, m_pSys->GetIamgeType(), m_InspData);		//Template 이미지 전처리

		Mat res;

		cv::matchTemplate((*pImage), m_Template, res, TM_CCOEFF_NORMED);
		
		double maxv;
		Point maxloc;
		
		int nstate[resultdata_Count];
		
		cv::minMaxLoc(res, 0, &maxv, 0, &maxloc);

		maxv *= 100;

		if (maxv == m_InspData.Standard)
		{
			nstate[ePassstate] = TRUE;
			nstate[ePreCent] = maxv;
			nstate[eCount] = result[ePass]++;
		}
		else
		{
			if (maxv > (m_InspData.Standard - m_InspData.Lower))
			{
				nstate[ePassstate] = TRUE;
				nstate[ePreCent] = maxv;
				nstate[eCount] = result[ePass]++;
			}
			else
			{
				nstate[ePassstate] = FALSE;
				nstate[ePreCent] = maxv;
				nstate[eCount] = result[eNonPass]++;

			}
		}
		
		CPoint Strat,End;
		Strat.x = maxloc.x;
		Strat.y = maxloc.y;
		End.x = maxloc.x + m_Template.cols;
		End.y = maxloc.y + m_Template.rows;
		

		if (m_pSys->GetLiveState() == FALSE) 
		{
			pDraw->Display();
			pDraw->DrawRoi(Strat, End);
		}
		m_pSys->GetDlgWnd()->PostMessage(UM_UPDATE_RESULT,static_cast<WPARAM>(nstate[ePass]), static_cast<LPARAM>(nstate[eCount]));
		m_pSys->GetDlgWnd()->PostMessage(UM_UPDATE_LLIST, static_cast<WPARAM>(nstate[ePass]), static_cast<LPARAM>(nstate[ePreCent]));
		m_pSys->m_InspDoneEvent.SetEvent();
		m_pSys->m_Count++;

		::LeaveCriticalSection(&m_csInsp);
	}
	
	m_pSys->m_eInspQuit.SetEvent();
	return 0;
}

void InspectionThread::SetThreadState(BOOL use)
{
	m_bThUse = use;
}

void InspectionThread::ReSetCount()
{
	for (int i = 0; i < result_Count; i++)
	{
		result[i] = 0;
	}
}

BOOL InspectionThread::GetTemplate(cv::Mat& Templ)
{
	CString str;
	str.Format(L"%s\\TemplateImage.bmp", TEMPLATEIMAGESAVE_PATH);
	String savePath = std::string(CT2CA(str));
	
	Templ = imread(savePath, IMREAD_COLOR);
	
	if (!(Templ.empty())) 
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
	
}

