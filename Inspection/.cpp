// Recipe.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "Recipe.h"
#include "afxdialogex.h"
#include "InspectionSystem.h"

InspectionSystem* m_pSystem;

// Recipe 대화 상자

IMPLEMENT_DYNAMIC(Recipe, CDialogEx)

Recipe::Recipe(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECIPE_DIALOG, pParent)
	, m_RadioImageType(0)
{

}

Recipe::~Recipe()
{
}

void Recipe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioImageType);
}


BEGIN_MESSAGE_MAP(Recipe, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_READ, &Recipe::OnBnClickedButtonImageRead)
END_MESSAGE_MAP()


// Recipe 메시지 처리기


void Recipe::OnBnClickedButtonImageRead()
{
	
}
