#pragma once
#include <iostream>
#include <vector>

//#include "opencv2/opencv.hpp"
#include "InspectionDefine.h"
#include "InspectionThread.h" //�˻� Thread
#include "CameraThread.h" //ī�޶� Thread
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

	cv::VideoCapture*	m_pVideoCap;					// ���� ī�޶� Class
	cv::Mat*			m_pFrame;						// ī�޶� ���� Class
	cv::Mat*			m_pCapture;						// ī�޶� ���� Class
	DrawWnd*			m_pDisplay;						// ��ο� Class
	ImageType			m_eImageType;					// �̹��� Ÿ�� enum
	InspData			m_InspData;
	CStdioFile			m_file;
	CPoint				m_startP;
	CPoint				m_endP;
	BOOL				m_bLiveState;
	BOOL				m_bLive;
public :
	CEvent				m_eCamQuit;				// ���� �̺�Ʈ
	CEvent				m_eInspQuit;			// ���� �̺�Ʈ
	CEvent				m_InspEvent; //ķ Thread �� �˻� Thread ����ȭ�뵵.
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

