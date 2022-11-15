// RecipeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "RecipeDlg.h"
#include "InspectionDlg.h"
#include "afxdialogex.h"

using namespace std;
using namespace cv;


// RecipeDlg 대화 상자

IMPLEMENT_DYNAMIC(RecipeDlg, CDialogEx)

RecipeDlg::RecipeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECIPE_DIALOG, pParent)
	, m_RadioImageType(0)
{

}

RecipeDlg::~RecipeDlg()
{
}

BOOL RecipeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	initialization(); //초기화
	
	return TRUE;
}

void RecipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECIPE_MAIN, m_Recipe_MainDisplay);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioImageType);
	DDX_Control(pDX, IDC_TOPX, m_TopX);
	DDX_Control(pDX, IDC_TOPY, m_TopY);
	DDX_Control(pDX, IDC_BOTTOMX, m_BottomX);
	DDX_Control(pDX, IDC_BOTTOMY, m_BottomY);
	DDX_Control(pDX, IDC_SLIDER_Th, m_Slider_TH);
	DDX_Control(pDX, IDC_SLIDER_He, m_Slider_HE);
	DDX_Control(pDX, IDC_SLIDER_Le, m_Slider_LE);
	DDX_Control(pDX, IDC_ThresHold, m_thresHold);
	DDX_Control(pDX, IDC_HighEdge, m_HighEdge);
	DDX_Control(pDX, IDC_LowEdge, m_LowEdge);

	DDX_Control(pDX, IDC_RESULT, m_Result);
	DDX_Control(pDX, IDC_EDIT_STANDARD, m_Standard);
	DDX_Control(pDX, IDC_EDIT_UPPER, m_upper);
	DDX_Control(pDX, IDC_EDIT_LOWER, m_Lower);
	DDX_Control(pDX, IDC_RESULT_VALUE, m_value);
}

void RecipeDlg::ExitSystem()
{
	//SafeDelete
	if (m_pDisplay != nullptr)
	{
		SAFE_DELETE(m_pDisplay)
	}
	if (m_pFream != nullptr)
	{
		SAFE_DELETE(m_pFream)
	}
}

void RecipeDlg::initialization()
{

	m_pDisplay = new DrawWnd;
	m_pFream = new cv::Mat;
	
	m_bLClicked = FALSE;
	m_bRClicked = FALSE;
	
	//CString str;

	m_Slider_TH.SetRange(0, 255);
	m_Slider_TH.SetLineSize(1);
	m_Slider_TH.SetPageSize(1);
	
	//str.Format(_T("%d"), m_Slider_TH.GetPos());
	//m_thresHold.SetWindowText(str);

	m_Slider_HE.SetRange(0, 255);
	
	m_Slider_HE.SetLineSize(1);
	m_Slider_HE.SetPageSize(1);
	
	//str.Format(_T("%d"), m_Slider_HE.GetPos());
	//m_HighEdge.SetWindowText(str);

	m_Slider_LE.SetRange(0, 255);
	m_Slider_LE.SetLineSize(1);
	m_Slider_LE.SetPageSize(1);

	//str.Format(_T("%d"), m_Slider_LE.GetPos());
	//m_LowEdge.SetWindowText(str);

	ReadRecipe();

	m_pDisplay->initialization(GetDlgItem(IDC_RECIPE_MAIN));
}


BEGIN_MESSAGE_MAP(RecipeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_READ, &RecipeDlg::OnBnClickedButtonImageRead)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_ZOOM_IN, &RecipeDlg::OnBnClickedZoomIn)
	ON_BN_CLICKED(IDC_ZOOM_OUT, &RecipeDlg::OnBnClickedZoomOut)
	ON_BN_CLICKED(IDC_ZOOM_FIT, &RecipeDlg::OnBnClickedZoomFit)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVE, &RecipeDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_APPLY_DATA, &RecipeDlg::OnBnClickedApplyData)
	ON_BN_CLICKED(IDC_Test, &RecipeDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_APPLY, &RecipeDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_CAPTURE, &RecipeDlg::OnBnClickedCapture)
END_MESSAGE_MAP()


// RecipeDlg 메시지 처리기


void RecipeDlg::OnBnClickedButtonImageRead()
{
	UpdateData(TRUE);
	
	switch (m_RadioImageType)
	{
	case 0:
		eImage = eImage_Color;
		break;
	case 1:
		eImage = eImage_Gray;
		break;
	case 2:
		eImage = eImage_Binary;
		break;
	case 3:
		eImage = eImage_Edge;
		break;
	}

	GetInspdata();

	m_pDisplay->ImageRead(*m_pFream, eImage, m_inspData,m_BackUp);
	m_pDisplay->Display();
	
}


void RecipeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pFream->empty()) { return; }
	GetCursorPos(&point);
	GetDlgItem(IDC_RECIPE_MAIN)->ScreenToClient(&point);
	
	m_bLClicked = TRUE;
	m_BakcLP = point;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void RecipeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pFream->empty()) { return; }
	
	GetCursorPos(&point);
	GetDlgItem(IDC_RECIPE_MAIN)->ScreenToClient(&point);

	if (m_bLClicked & TRUE) { m_bLClicked = FALSE; }
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void RecipeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pFream->empty()) { return; }
	GetCursorPos(&point);
	GetDlgItem(IDC_RECIPE_MAIN)->ScreenToClient(&point);
	CString str;
	
	if (m_bLClicked & TRUE) 
	{
		m_pDisplay->move(m_BakcLP, point);
		m_pDisplay->Display();
	}
	if (m_bRClicked & TRUE) 
	{
		m_pDisplay->SetRoiPoint(m_BakcRP, point);
		
		//픽처 컨트롤의 좌표를 이미지 좌표로 변환한다.
		m_pDisplay->ConvertPictureToImage(m_BakcRP, m_back);
		m_pDisplay->ConvertPictureToImage(point, m_now);

		str.Format(L"%d", m_back.x);
		m_TopX.SetWindowText(str);

		str.Format(L"%d", m_back.y);
		m_TopY.SetWindowText(str);

		str.Format(L"%d", m_now.x);
		m_BottomX.SetWindowText(str);

		str.Format(L"%d", m_now.y);
		m_BottomY.SetWindowText(str);
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void RecipeDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pFream->empty()) { return; }

	GetCursorPos(&point);
	GetDlgItem(IDC_RECIPE_MAIN)->ScreenToClient(&point);
	m_bRClicked = TRUE;
	m_BakcRP = point;

	if (nFlags & MK_CONTROL && m_bRClicked)
	{
		m_pDisplay->ChangeRoiflag(FALSE);
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void RecipeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pFream->empty()) { return; }

	GetCursorPos(&point);
	GetDlgItem(IDC_RECIPE_MAIN)->ScreenToClient(&point);

	if (m_bRClicked & TRUE) { m_bRClicked = FALSE; }

	CDialogEx::OnRButtonUp(nFlags, point);
}

void RecipeDlg::RecipeFileSave()
{
	UpdateData(TRUE);

	CString str;
	int index = 0;
	m_savedata[index].Format(L"%d\n", m_back.x); index++;
	m_savedata[index].Format(L"%d\n", m_back.y); index++;
	m_savedata[index].Format(L"%d\n", m_now.x); index++;
	m_savedata[index].Format(L"%d\n", m_now.y); index++;
	m_savedata[index].Format(L"%d\n", m_Slider_TH.GetPos()); index++;
	m_savedata[index].Format(L"%d\n", m_Slider_HE.GetPos()); index++;
	m_savedata[index].Format(L"%d\n", m_Slider_LE.GetPos()); index++;
	m_Standard.GetWindowText(str);
	m_savedata[index].Format(L"%s\n", str); index++;
	m_upper.GetWindowText(str);
	m_savedata[index].Format(L"%s\n", str); index++;
	m_Lower.GetWindowText(str);
	m_savedata[index].Format(L"%s\n", str); index++;
	m_savedata[index].Format(L"%d\n", m_RadioImageType);
	

	CString Path;
	Path.Format(_T("%s\\Recipe.txt"),INSPECTION_PATH);
	BOOL bOpen = m_file.Open(Path, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		AfxMessageBox(_T("Open Error"));
	}
	for (int i = 0; i < 10; i++) 
	{
		m_file.WriteString(m_savedata[i]);
	}
	m_file.Close();
	AfxMessageBox(L"파일 저장이 완료되었습니다.");


}

void RecipeDlg::ReadRecipe()
{
	setlocale(LC_ALL, "");

	CString Path , str;
	Path.Format(_T("%s\\Recipe.txt"), INSPECTION_PATH);
	BOOL bOpen = m_file.Open(Path, CStdioFile::modeRead | CFile::typeText);
	if (!bOpen)
	{
		AfxMessageBox(_T("Open Error"));
	}
	for (int i = 0; i < 10; i++)
	{
		m_file.ReadString(m_savedata[i]);
	}
	int index = 0;
	int pos = 0;
	m_back.x = _ttoi(m_savedata[index]); index++;
	m_inspData.topX = m_back.x;
	str.Format(L"%d", m_back.x);
	m_TopX.SetWindowText(str);
	
	m_back.y = _ttoi(m_savedata[index]); index++;
	m_inspData.topY = m_back.y;
	str.Format(L"%d", m_back.y);
	m_TopY.SetWindowText(str);
	
	m_now.x = _ttoi(m_savedata[index]);  index++;
	m_inspData.BotX = m_now.x;
	str.Format(L"%d", m_now.x);
	m_BottomX.SetWindowText(str);
	
	m_now.y = _ttoi(m_savedata[index]);  index++;
	m_inspData.BotY = m_now.y;
	str.Format(L"%d", m_now.y);
	m_BottomY.SetWindowText(str);

	m_nthresHold = _ttoi(m_savedata[index]);
	m_inspData.ThresHold = m_nthresHold;
	m_Slider_TH.SetPos(m_nthresHold); index++;
	str.Format(_T("%d"), m_nthresHold);
	m_thresHold.SetWindowText(str);
	
	m_nHighEdge = _ttoi(m_savedata[index]);
	m_inspData.EdgeHigh = m_nHighEdge;
	m_Slider_HE.SetPos(m_nHighEdge); index++;
	str.Format(_T("%d"), m_nHighEdge);
	m_HighEdge.SetWindowText(str);

	
	m_nLowEdge = _ttoi(m_savedata[index]);
	m_inspData.EdgeLOW = m_nLowEdge;
	m_Slider_LE.SetPos(m_nLowEdge); index++;
	str.Format(_T("%d"), m_nLowEdge);
	m_LowEdge.SetWindowText(str);

	m_Standard.SetWindowText(m_savedata[index]); 
	m_inspData.Standard = _ttoi(m_savedata[index]); index++;

	m_upper.SetWindowText(m_savedata[index]);
	m_inspData.Upper = _ttoi(m_savedata[index]); index++;

	m_Lower.SetWindowText(m_savedata[index]);
	m_inspData.Lower = _ttoi(m_savedata[index]); index++;

	m_RadioImageType = _ttoi(m_savedata[index]);
	m_inspData.eImageType = _ttoi(m_savedata[index]);

	UpdateData(FALSE);

	//m_pDisplay->ConvertImageToPicture(m_back, m_readback);
	//m_pDisplay->ConvertImageToPicture(m_now, m_readnow);

	//m_pDisplay->SetROIPoint(m_readback, m_readnow);
	//m_pDisplay->Display();

	m_file.Close();
}

void RecipeDlg::GetInspdata()
{
	UpdateData(TRUE);

	CString str;
	m_TopX.GetWindowText(str);
	m_inspData.topX = _ttoi(str);

	m_TopY.GetWindowText(str);
	m_inspData.topY = _ttoi(str);

	m_BottomX.GetWindowText(str);
	m_inspData.BotX = _ttoi(str);

	m_BottomY.GetWindowText(str);
	m_inspData.BotY = _ttoi(str);

	m_inspData.ThresHold = m_Slider_TH.GetPos();

	m_inspData.EdgeHigh = m_Slider_HE.GetPos();

	m_inspData.EdgeLOW = m_Slider_LE.GetPos();

	m_Standard.GetWindowText(str);
	m_inspData.Standard = _ttoi(str);

	m_upper.GetWindowText(str);
	m_inspData.Upper = _ttoi(str);

	m_Lower.GetWindowText(str);
	m_inspData.Lower = _ttoi(str);

	m_inspData.eImageType = m_RadioImageType;
}


void RecipeDlg::OnBnClickedZoomIn()
{
	if (m_pFream->empty()) { return; }
	m_pDisplay->Zoom(1, 0);
}


void RecipeDlg::OnBnClickedZoomOut()
{
	if (m_pFream->empty()) { return; }
	m_pDisplay->Zoom(-1, 0);
}


void RecipeDlg::OnBnClickedZoomFit()
{
	if (m_pFream->empty()) { return; }
	m_pDisplay->SetRetore();
	m_pDisplay->Display();
}


void RecipeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString str;
	if (IDC_SLIDER_Th == pScrollBar->GetDlgCtrlID())
	{
		str.Format(_T("%d"), m_Slider_TH.GetPos());
		m_thresHold.SetWindowText(str);
	}
	if (IDC_SLIDER_He == pScrollBar->GetDlgCtrlID())
	{
		str.Format(_T("%d"), m_Slider_HE.GetPos());
		m_HighEdge.SetWindowText(str);
	}
	if (IDC_SLIDER_Le == pScrollBar->GetDlgCtrlID())
	{
		str.Format(_T("%d"), m_Slider_LE.GetPos());
		m_LowEdge.SetWindowText(str);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void RecipeDlg::OnBnClickedSave()
{
	RecipeFileSave();
}


void RecipeDlg::OnBnClickedApplyData()
{
	UpdateData(TRUE);
	
	switch (m_RadioImageType)
	{
	case 0:
		eImage = eImage_Color;
		break;
	case 1:
		eImage = eImage_Gray;
		break;
	case 2:
		eImage = eImage_Binary;
		break;
	case 3:
		eImage = eImage_Edge;
		break;
	}

	GetInspdata();
	Mat Temp = m_pFream->clone();
	m_pDisplay->SetImage(Temp, eImage, m_inspData);
	m_pDisplay->SetImageRread(TRUE);
	m_pDisplay->Display();
}


void RecipeDlg::OnBnClickedTest()
{
	GetInspdata();
	
	CString str;
	str.Format(L"%s\\TemplateImage.bmp", TEMPLATEIMAGESAVE_PATH);
	String savePath = std::string(CT2CA(str));

	Mat Capture = imread(savePath, IMREAD_COLOR);
	
	if (Capture.empty()) 
	{
		AfxMessageBox(L"이미지를 읽어오는데 실패 했습니다.");
		return;
	}
	
	m_pDisplay->ChangeImageType((*m_pFream), eImage, m_inspData);		//영상 전처리
	m_pDisplay->ChangeImageType(Capture, eImage, m_inspData);		//Template 이미지 전처리

	Mat res;

	cv::matchTemplate((*m_pFream), Capture, res, TM_CCOEFF_NORMED);

	double maxv;
	Point maxloc;
	
	cv::minMaxLoc(res, 0, &maxv, 0, &maxloc);
	
	maxv *= 100;

	if(maxv == m_inspData.Standard)
	{
		m_Result.SetWindowText(L"합  격");
		str.Format(L"%f", maxv);
		m_value.SetWindowText(str);
	}
	else
	{
		if (maxv > (m_inspData.Standard - m_inspData.Lower)) 
		{
			m_Result.SetWindowText(L"합  격");
			str.Format(L"%f", maxv);
			m_value.SetWindowText(str);
		}
		else 
		{
			m_Result.SetWindowText(L"불 합 격");
			str.Format(L"%f", maxv);
			m_value.SetWindowText(str);
		}
		
	}
	CPoint Strat, End;
	Strat.x = maxloc.x;
	Strat.y = maxloc.y;
	End.x = maxloc.x + Capture.cols;
	End.y = maxloc.y + Capture.rows;

	m_pDisplay->DrawRoi(Strat, End);

	AfxMessageBox(L"검사가 종료 되었습니다.");
}


void RecipeDlg::OnBnClickedApply()
{
	RecipeFileSave();
	m_pMainDlg->PostMessage(UM_RECIPE_UPDATE, NULL, NULL);
}


void RecipeDlg::OnBnClickedCapture()
{
	if (m_pFream->empty())
	{
		AfxMessageBox(L"이미지가 비어있습니다.");
		return;
	}
	GetInspdata();

	Mat Temp = m_pFream->clone();
	Mat Capture = Temp(Rect(Point(m_inspData.topX, m_inspData.topY), Point(m_inspData.BotX, m_inspData.BotY))).clone();
	
	CString str;
	str.Format(L"%s\\TemplateImage.bmp", TEMPLATEIMAGESAVE_PATH);
	cv::String savePath = std::string(CT2CA(str));
	imwrite(savePath, Capture);

	AfxMessageBox(L"이미지가 저장되었습니다.");
}

void RecipeDlg::GetDlgToCWnd(CWnd* wnd)
{
	m_pMainDlg = wnd;
}

