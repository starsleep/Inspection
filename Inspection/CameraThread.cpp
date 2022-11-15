#include "pch.h"
#include "InspectionSystem.h"
#include "CameraThread.h"

IMPLEMENT_DYNCREATE(CameraThread, CWinThread);

InspectionSystem* m_pSysCam;

BOOL CameraThread::InitInstance()
{
    //Get MainSystem Pointer;
    m_pSysCam = InspectionSystem::GetInstance();
    ::InitializeCriticalSection(&m_csCam);




    return TRUE;
}

int CameraThread::Exitlnstance()
{
    m_bThUse = FALSE;
    return 0;
}

int CameraThread::Run()
{
    DrawWnd* pDraw = m_pSysCam->GetDrawWnd();
    cv::Mat* pImage = m_pSysCam->GetMat();
    cv::VideoCapture* pVideoCap = m_pSysCam->GetVideoCapture();
   
    if (pDraw == nullptr || pImage == nullptr || pVideoCap == nullptr) 
    {
        m_pSysCam->m_eCamQuit.SetEvent();
        return -1;
    }

    while(m_bThUse) 
    {
        //임계구역 
        ::EnterCriticalSection(&m_csCam);

        if (pDraw == nullptr || pImage == nullptr || pVideoCap == nullptr)
        {
            ::LeaveCriticalSection(&m_csCam);
        }

        pVideoCap->read(*pImage);

        pDraw->SetImage(*pImage, m_pSysCam->GetIamgeType(), m_pSysCam->GetInspdata());

        if(m_pSysCam->GetLiveState()== TRUE) 
        {
            pDraw->Display();
            //pDraw->DrawRoi(start, End);
            
        }
        else 
        {
            ::WaitForSingleObject(m_pSysCam->m_InspDoneEvent, INFINITE);
            m_pSysCam->m_InspDoneEvent.ResetEvent();
        }
        
        m_pSysCam->m_InspEvent.SetEvent();
        ::LeaveCriticalSection(&m_csCam);
        Sleep(30);
    }

    m_pSysCam->m_eCamQuit.SetEvent();
    return 0;
}

void CameraThread::SetThreadState(BOOL buse)
{
    m_bThUse = buse;
}
