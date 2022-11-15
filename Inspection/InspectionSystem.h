#pragma once
#include <iostream>
#include <vector>

//#include "opencv2/opencv.hpp"
#include "InspectionDefine.h"
#include "InspectionThread.h" //검사 Thread
#include "CameraThread.h" //카메라 Thread
#include "DrawWnd.h"



template <typename T> T* TemplateSingleton<T>::m_pInstance = nullptr;

class InspectionSystem : public CDialogEx , public TemplateSingleton<InspectionSystem> //singleton pattren
{
private :
	//ui pointer
	CWnd*				m_pDlg;
	CWnd*				m_pMainDisplay;
	CameraThread*		m_pCamThread;
	InspectionThread*	m_pInspThread;

	cv::VideoCapture*	m_pVideoCap;					// 비디오 카메라 Class
	cv::Mat*			m_pFrame;						// 카메라 영상 Class
	cv::Mat*			m_pCapture;						// 카메라 영상 Class
	DrawWnd*			m_pDisplay;						// 드로우 Class
	ImageType			m_eImageType;					// 이미지 타입 enum
	InspData			m_InspData;
	CStdioFile			m_file;
	CPoint				m_startP;
	CPoint				m_endP;
	BOOL				m_bLiveState;
	BOOL				m_bLive;
public :
	CEvent				m_eCamQuit;				// 종료 이벤트
	CEvent				m_eInspQuit;			// 종료 이벤트
	CEvent				m_InspEvent; //캠 Thread 와 검사 Thread 동기화용도.
	CEvent				m_InspDoneEvent;
	int					m_Count;

	InspectionSystem(){}

	~InspectionSystem(){}

	void				initialization(std::vector<void*>& vpptr);

	void				ExitSystem();

	BOOL				OpenCamera(int CamIndex);

	CameraThread*		GetpThread() { return m_pCamThread; };
	
	DrawWnd*			GetDrawWnd() { return m_pDisplay; };
	
	cv::Mat*			GetMat() { return m_pFrame; }
	
	cv::VideoCapture*	GetVideoCapture() { return m_pVideoCap; }

	CWnd*				GetDlgWnd() { return m_pDlg; }
	
	void				SetIamgeType(ImageType eIamge);

	ImageType			GetIamgeType() { return m_eImageType; }

	void				SetLiveState(BOOL bstate) {m_bLiveState = bstate;}

	BOOL				GetLiveState() { return m_bLiveState; }

	void				LIVE();

	void				LIVEFit();
	
	void				ZoomIn();
	
	void				ZoomOut();

	void				move(CPoint cpBack, CPoint cpnow);

	void				StopCamThread();

	InspData			ReadRecipe();

	InspData			GetInspdata(){return m_InspData;}

	void				StartInspetion();

	void				ImageSave();

	void				StopInspetionThread();
};

