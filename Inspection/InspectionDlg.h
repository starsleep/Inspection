#pragma once

#include "InspectionSystem.h"
#include "RecipeDlg.h"

using namespace cv;

// CInspectionDlg 대화 상자
class CInspectionDlg : public CDialogEx
{
// 생성입니다.
public:
	CInspectionDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	
	//Thread 관련 함수
	//CWinThread* m_pCamThread;
	InspectionSystem *m_pSys;
	RecipeDlg* m_Recipedlg;

	//MFC 관련 함수 및 변수
	void				 initialization();
	CButton				m_Uibutton[eCam_Count];						// UI에 적용되는 버튼
	CStatic				m_Main_Display;								// Main Picturecontrol
	CWnd*				m_pMain_Display;
	int					m_RadioImageType;
	CEdit				ECam_Index;
	BOOL				m_bLCliked;
	CPoint				m_BackP;
	CListCtrl			m_ResultList;
	InspData			m_InspData;
	int					m_Result[resultdata_Count];
	BOOL				m_inspstate;
	CFont				m_Font, m_Font1;

	afx_msg void		OnBnClickedButConnetCam();
	
	afx_msg void		OnDestroy();
	afx_msg void		OnClose();
	afx_msg void		OnBnClickedButtonExit();
	afx_msg void		OnBnClickedButtonLive();

	
	afx_msg void		OnBnClickedButtonZoomIn();
	afx_msg void		OnBnClickedButtonZoomOut();
	afx_msg void		OnBnClickedButtonZoomFit();
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void		OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void		OnBnClickedButtonRecipe();
	afx_msg void		OnBnClickedButtonStop();
	void				UpdateResult();
	void				UpdateControl();

	void				UpdateList();
	CInspectionDlg*		GetCInspectionDlg() { return this; };
	CStatic				m_Standard;
	CStatic				m_Upper;
	CStatic				m_Lower;
	CStatic				m_Treshold;
	CStatic				m_EdgeHigh;
	CStatic				m_EdgeLow;
	CStatic				m_PassCount;
	CStatic				m_NonPassCount;
	
	afx_msg void OnBnClickedButtonInsp();

	afx_msg LRESULT OnDisplayResult(WPARAM wParam, LPARAM lParam);
	CStatic m_JUDE;
	afx_msg void OnBnClickedButtonImageSave();

	afx_msg LRESULT OnReReadRecipe(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayList(WPARAM wParam, LPARAM lParam);
	
};
