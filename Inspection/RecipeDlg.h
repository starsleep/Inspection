#pragma once
#include "DrawWnd.h"
#include "InspectionDefine.h"

// RecipeDlg 대화 상자

class RecipeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RecipeDlg)

public:
	RecipeDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RecipeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECIPE_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	CWnd*			m_pMainDlg;
	DrawWnd*		m_pDisplay;
	CStatic			m_Recipe_MainDisplay;
	cv::Mat*		m_pFream;
	cv::Mat			m_BackUp;
	BOOL			m_bLClicked;
	BOOL			m_bRClicked;
	CPoint			m_BakcLP;
	CPoint			m_BakcRP;
	int m_nthresHold, m_nHighEdge, m_nLowEdge;

	//이미지 좌표계로 바꾼 좌표
	CPoint m_back, m_now, m_readback , m_readnow;
	
	CStdioFile m_file;

	CString m_savedata[11];

	InspData m_inspData;

	ImageType eImage;

public:
	
	void ExitSystem();

	void initialization();

	afx_msg void OnBnClickedButtonImageRead();
	int m_RadioImageType;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void RecipeFileSave();
	void ReadRecipe();
	void GetInspdata();
	
	
	CStatic m_TopX , m_TopY, m_BottomX, m_BottomY;

	afx_msg void OnBnClickedZoomIn();
	afx_msg void OnBnClickedZoomOut();
	afx_msg void OnBnClickedZoomFit();
	CSliderCtrl m_Slider_TH, m_Slider_HE, m_Slider_LE;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic m_thresHold, m_HighEdge, m_LowEdge;
	
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedApplyData();
	CStatic m_Result;
	afx_msg void OnBnClickedTest();
	CEdit m_Standard;
	CEdit m_upper;
	CEdit m_Lower;
	CStatic m_value;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCapture();
	void GetDlgToCWnd(CWnd* wnd);
};
