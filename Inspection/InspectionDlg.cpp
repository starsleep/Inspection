
// InspectionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Inspection.h"
#include "InspectionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define pMainSystem	InspectionSystem::GetInstance();

using namespace std;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CInspectionDlg 대화 상자



CInspectionDlg::CInspectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSPECTION_DIALOG, pParent)
	, m_RadioImageType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInspectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_DISPLAY, m_Main_Display);
	DDX_Control(pDX, IDC_BUT_Connet_Cam, m_Uibutton[eCam_Connet]);
	DDX_Control(pDX, IDC_BUTTON_Live, m_Uibutton[eCam_Live]);
	DDX_Control(pDX, IDC_BUTTON_Stop, m_Uibutton[eCam_Stop]);
	DDX_Control(pDX, IDC_BUTTON_Insp, m_Uibutton[eCam_StratInsp]);
	//DDX_Control(pDX, IDC_BUTTON_Insp, m_Uibutton[eCam_Capture]);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_Uibutton[eCam_ZoomIn]);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_Uibutton[eCam_Zoomout]);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_FIT, m_Uibutton[eCam_ZoomFit]);
	DDX_Control(pDX, IDC_BUTTON_Recipe, m_Uibutton[eCam_Recipe]);
	DDX_Control(pDX, IDC_EDIT_Cam_Index, ECam_Index);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioImageType);
	DDX_Control(pDX, IDC_LIST_RESUIT, m_ResultList);
	DDX_Control(pDX, IDC_STANDARD, m_Standard);
	DDX_Control(pDX, IDC_UPPER, m_Upper);
	DDX_Control(pDX, IDC_LOWER, m_Lower);
	DDX_Control(pDX, IDC_ThresHold, m_Treshold);
	DDX_Control(pDX, IDC_EDGEHIGH, m_EdgeHigh);
	DDX_Control(pDX, IDC_EDGELOW, m_EdgeLow);
	DDX_Control(pDX, IDC_PASS_COUNT, m_PassCount);
	DDX_Control(pDX, IDC_NON_PASS_COUNT, m_NonPassCount);
	DDX_Control(pDX, IDC_JUDE, m_JUDE);
}

BEGIN_MESSAGE_MAP(CInspectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_Connet_Cam, &CInspectionDlg::OnBnClickedButConnetCam)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CInspectionDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_Live, &CInspectionDlg::OnBnClickedButtonLive)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CInspectionDlg::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CInspectionDlg::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_FIT, &CInspectionDlg::OnBnClickedButtonZoomFit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_Recipe, &CInspectionDlg::OnBnClickedButtonRecipe)
	ON_BN_CLICKED(IDC_BUTTON_Stop, &CInspectionDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_Insp, &CInspectionDlg::OnBnClickedButtonInsp)
	ON_MESSAGE(UM_UPDATE_RESULT, &CInspectionDlg::OnDisplayResult)
	ON_MESSAGE(UM_RECIPE_UPDATE, &CInspectionDlg::OnReReadRecipe)
	ON_MESSAGE(UM_UPDATE_LLIST, &CInspectionDlg::OnDisplayList)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SAVE, &CInspectionDlg::OnBnClickedButtonImageSave)
END_MESSAGE_MAP()


// CInspectionDlg 메시지 처리기

BOOL CInspectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//초기화 함수
	initialization();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CInspectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE) 
	{
		//SAFE_DELETE(m_pVideoCap);
		this->PostMessage(WM_CLOSE);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CInspectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CInspectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CInspectionDlg::initialization()
{
	//Main System 초기화
	m_pSys = InspectionSystem::GetInstance();

	m_InspData = m_pSys->ReadRecipe();
	
	m_inspstate = TRUE;
	
	UpdateControl();

	//vector에 포인터를 담아서 Mainsystem으로 전달
	vector<void*> vpDlgptr;
	vpDlgptr.push_back(reinterpret_cast<void*>(this));
	vpDlgptr.push_back(reinterpret_cast<void*>(GetDlgItem(IDC_MAIN_DISPLAY)));
	m_pSys->initialization(vpDlgptr);
	GetDlgItem(IDC_BUTTON_Live)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Stop)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Insp)->EnableWindow(FALSE);

	// List Control 초기화
	CRect rt; 
	m_ResultList.GetWindowRect(&rt);
	m_ResultList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 리스트 컨트롤에 선표시 및 Item 선택시 한행 전체 선택
	m_ResultList.InsertColumn(0, TEXT("Time"), LVCFMT_LEFT, rt.Width() * 0.25); // 첫번째
	m_ResultList.InsertColumn(1, TEXT("No"), LVCFMT_CENTER, rt.Width() * 0.25); // 두번째
	m_ResultList.InsertColumn(2, TEXT("결과"), LVCFMT_CENTER, rt.Width() * 0.25); // 세번째
	m_ResultList.InsertColumn(3, TEXT("유사도"), LVCFMT_CENTER, rt.Width() * 0.25); // 세번째

	//UI Font 초기화
	m_Font.CreateFont(50, // nHeight
		30, // nWidth
		0, // nEscapement
		0, // nOrientation
		1, // nWeight
		0, // bItalic
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		L"휴먼매직체"); // lpszFacename
	GetDlgItem(IDC_JUDE)->SetFont(&m_Font);

	m_Font1.CreateFont(30, // nHeight
		20, // nWidth
		0, // nEscapement
		0, // nOrientation
		1, // nWeight
		0, // bItalic
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		L"휴먼매직체"); // lpszFacename
	
	GetDlgItem(IDC_STATIC_PASS)->SetFont(&m_Font1);
	GetDlgItem(IDC_STATIC_NONPASS)->SetFont(&m_Font1);


}

void CInspectionDlg::OnBnClickedButConnetCam()
{
	//카메라 연결해주는 기능
	CString strCamIdex = _T("");
	ECam_Index.GetWindowText(strCamIdex);
	int nCamIdex = _ttoi(strCamIdex);
	BOOL bCamstate = m_pSys->OpenCamera(nCamIdex);
	if (bCamstate == FALSE)
	{
		AfxMessageBox(L"카메라 연결 실패");
		return;
	}
	else 
	{
		strCamIdex.Format(L"%d번 카메라 연결 완료", nCamIdex);
		AfxMessageBox(strCamIdex);
	}
	GetDlgItem(IDC_BUTTON_Live)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Stop)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Insp)->EnableWindow(TRUE);
}


void CInspectionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}


void CInspectionDlg::OnClose()
{
	//MFC 종료 할 코드 적기
	if (AfxMessageBox(L"프로그램을 종료하시겠습니까 ?", MB_YESNO | MB_ICONQUESTION) == IDYES) 
	{
		m_pSys->ExitSystem();
		m_pSys->DestroyInstance();
		AfxMessageBox(L"프로그램 종료 완료");
	}
	else 
	{
		return;
	}
	CDialogEx::OnClose();
}



void CInspectionDlg::OnBnClickedButtonExit()
{
	
}


void CInspectionDlg::OnBnClickedButtonLive()
{
	UpdateData(TRUE);

	switch (m_RadioImageType)
	{
	case 0:
		m_pSys->SetIamgeType(eImage_Color);
		break;
	case 1:
		m_pSys->SetIamgeType(eImage_Gray);
		break;
	case 2:
		m_pSys->SetIamgeType(eImage_Binary);
		break;
	case 3:
		m_pSys->SetIamgeType(eImage_Edge);
		break;
	}
	m_pSys->LIVE();
}


void CInspectionDlg::OnBnClickedButtonZoomIn()
{
	m_pSys->ZoomIn();
}


void CInspectionDlg::OnBnClickedButtonZoomOut()
{
	m_pSys->ZoomOut();
}


void CInspectionDlg::OnBnClickedButtonZoomFit()
{
	m_pSys->LIVEFit();
}


void CInspectionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&point);
	GetDlgItem(IDC_MAIN_DISPLAY)->ScreenToClient(&point);
	
	m_bLCliked = TRUE;
	m_BackP = point;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CInspectionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bLCliked)
	{
		m_bLCliked = FALSE;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}



void CInspectionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&point);
	GetDlgItem(IDC_MAIN_DISPLAY)->ScreenToClient(&point);
	if (m_bLCliked & TRUE) 
	{
		m_pSys->move(m_BackP, point);
		m_BackP = point;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CInspectionDlg::OnBnClickedButtonRecipe()
{
	m_Recipedlg = new RecipeDlg();
	m_Recipedlg->Create(IDD_RECIPE_DIALOG, this);
	m_Recipedlg->ShowWindow(SW_SHOW);
	m_Recipedlg->GetDlgToCWnd(reinterpret_cast<CWnd*>(this));
}


void CInspectionDlg::OnBnClickedButtonStop()
{
	m_pSys->StopCamThread();
	m_pSys->StopInspetionThread();
}

void CInspectionDlg::UpdateResult()
{
	CString str;

	if (m_Result[ePassstate] == 1) 
	{
		str.Format(L"합 격");
		m_JUDE.SetWindowText(str);

		str.Format(L"%d", m_Result[ePreCent]);
		m_PassCount.SetWindowText(str);
	}
	else 
	{
		str.Format(L"불 합 격");
		m_JUDE.SetWindowText(str);

		str.Format(L"%d", m_Result[ePreCent]);
		m_NonPassCount.SetWindowText(str);
	}
	
	//m_PassCount.SetWindowText(m_Result[1]);
	//m_NonPassCount.SetWindowText(m_Result[2]);
}

void CInspectionDlg::UpdateControl()
{
	CString str;

	str.Format(_T("%d"), m_InspData.ThresHold);
	m_Treshold.SetWindowText(str);

	str.Format(_T("%d"), m_InspData.EdgeHigh);
	m_EdgeHigh.SetWindowText(str);

	str.Format(_T("%d"), m_InspData.EdgeLOW);
	m_EdgeLow.SetWindowText(str);

	str.Format(_T("%d"), m_InspData.Standard);
	m_Standard.SetWindowText(str);

	str.Format(_T("%d"), m_InspData.Upper);
	m_Upper.SetWindowText(str);

	str.Format(_T("%d"), m_InspData.Lower);
	m_Lower.SetWindowText(str);
}

void CInspectionDlg::UpdateList()
{
	CString strTime , strTemp;
	CTime cTime = CTime::GetCurrentTime();

	strTime.Format(L"%02d시 %02d분", cTime.GetHour(), cTime.GetMinute());

	m_ResultList.InsertItem(0, LPCTSTR(strTime));
	strTemp.Format(L"%d", m_pSys->m_Count);
	m_ResultList.SetItem(0, 1, LVFIF_TEXT, LPCTSTR(strTemp), NULL, NULL, NULL, NULL);
	
	strTemp.Format(L"%d %% ", m_Result[ePreCent]);
	m_ResultList.SetItem(0, 2, LVFIF_TEXT, LPCTSTR(strTemp), NULL, NULL, NULL, NULL);

	if (m_Result[ePassstate] == 1) { strTemp.Format(L"합 격"); }
	else if (m_Result[ePassstate] == 0) { strTemp.Format(L"불 합 격"); }
	m_ResultList.SetItem(0, 3, LVFIF_TEXT, LPCTSTR(strTemp), NULL, NULL, NULL, NULL);

}


void CInspectionDlg::OnBnClickedButtonInsp()
{
	if (m_inspstate == TRUE)
	{
		
		m_pSys->LIVE();
		m_pSys->StartInspetion();
		m_inspstate = FALSE;
		m_Uibutton[eCam_StratInsp].SetWindowText(L"Inspetion Start");
	}
	else if (m_inspstate == FALSE) 
	{
		m_pSys->StopInspetionThread();
		m_pSys->m_InspDoneEvent.SetEvent();
		m_pSys->StopCamThread();
		m_inspstate = TRUE;
		m_Uibutton[eCam_StratInsp].SetWindowText(L"Inspetion Stop");
	}
}

LRESULT CInspectionDlg::OnDisplayResult(WPARAM wParam, LPARAM lParam)
{
	m_Result[ePassstate] = static_cast<int>(wParam);
	m_Result[ePreCent] = static_cast<int>(lParam);
	
	UpdateResult();
	return LRESULT();
}

LRESULT CInspectionDlg::OnDisplayList(WPARAM wParam, LPARAM lParam)
{
	m_Result[ePassstate] = static_cast<int>(wParam);
	m_Result[ePreCent] = static_cast<int>(lParam);

	UpdateList();
	return LRESULT();
}


void CInspectionDlg::OnBnClickedButtonImageSave()
{
	m_pSys->ImageSave();
}

LRESULT CInspectionDlg::OnReReadRecipe(WPARAM wParam, LPARAM lParam)
{
	m_InspData = m_pSys->ReadRecipe();

	UpdateControl();
	return LRESULT();
}
