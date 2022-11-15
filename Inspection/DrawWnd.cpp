#include "pch.h"
#include "DrawWnd.h"

using namespace cv;


DrawWnd::DrawWnd(){}

DrawWnd::~DrawWnd(){}

BOOL DrawWnd::initialization(CWnd* Wnd)
{
	//내부적으로 picture 컨트롤 포인터와 사이즈를 받아온다
	GetWnd(Wnd);

	//이미지 원점 및 움직인 거리 초기화
	m_origin_p.Xsal = 0;
	m_origin_p.Ysal = 0;
	m_move_p.Xsal = 0;
	m_move_p.Ysal = 0;

	m_movelenght.Xsal = 0;
	m_movelenght.Ysal = 0;

	m_Roiflag = FALSE;
	m_zoomflag = FALSE;
	m_THFlag = FALSE;
	m_ImageRead = FALSE;
	m_fScale = 1.0f;
	return TRUE;
}

void DrawWnd::GetWnd(CWnd* cWnd)
{
	m_wnd = cWnd;
	m_wnd->GetClientRect(&m_PictureSize);
}

//이미지를 읽어오는 방법 2가지 획득 / 로드
//로드
BOOL DrawWnd::ImageRead(Mat& Image, ImageType eImagetype , InspData sInspData, Mat& BackUp)
{
	//파일 선택창 open
	CFileDialog file(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, L"bmp image (*.bmp)|*.bmp||");
	CString path;
	cv::String cvpath;
	
	if (file.DoModal() == IDOK)
	{
		//file path
		path = file.GetPathName();

		//CString to cv::String
		cvpath = cv::String(CT2CA(path));

		//imread
		Mat Temp;
		Temp = imread(cvpath, IMREAD_COLOR);
		Image = Temp.clone();
		//BackUp = Image.clone();
		
		if(Image.empty())
		{
			//이미지가 비어 있을경우 리턴
			AfxMessageBox(L"이미지를 읽어오는데 실패 했습니다.");
			return FALSE;
		}
		SetImage(Temp, eImagetype, sInspData);
		m_ImageRead = TRUE;
		return TRUE;
	}
	else
	{
		AfxMessageBox(L"파일을 선택하지 못했습니다");
		return FALSE;
	}
}
//획득
void DrawWnd::SetImage(Mat& image, ImageType eImagetype, InspData sInspData)
{
	//m_Image = image.clone();
	switch (eImagetype)
	{
	case eImage_Color:
		break;

	case eImage_Gray:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
		}
		break;

	case eImage_Binary:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
		}
		else
		{
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);

		}
		break;

	case eImage_Edge:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
			Canny(image, image, sInspData.EdgeLOW, sInspData.EdgeHigh);
		}
		else
		{
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
			Canny(image, image, sInspData.EdgeLOW, sInspData.EdgeHigh);
		}
		break;
	}
	m_Image = image.clone();
	SetImageSize();
	m_ImageRead = FALSE;
}

void DrawWnd::Display()
{
	//Get pic DC
	CClientDC PicDC(m_wnd);
	
	//Mat Temp = m_Image.clone();
	//Bmp 헤더정의
	m_BitMapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + (dByte * sizeof(RGBQUAD))];
	
	createBIT();

	m_memDC.CreateCompatibleDC(&PicDC);

	m_Bmp.CreateCompatibleBitmap(&PicDC, m_PictureSize.right, m_PictureSize.bottom);

	m_OldBmp = m_memDC.SelectObject(&m_Bmp);

	//검은색 Bmp
	m_memDC.PatBlt(0, 0, m_PictureSize.right, m_PictureSize.bottom, BLACKNESS);

	//resize memDC
	::SetStretchBltMode(m_memDC, COLORONCOLOR);
	::StretchDIBits
	(
		m_memDC,							// 출력 대상의 DC 핸들
		m_PictureSize.left,					// 출력 대상의 좌상귀 x 좌표
		m_PictureSize.top,					// 출력 대상의 좌상귀 y 좌표
		m_PictureSize.right,				// 출력 대상의 사각형 너비
		m_PictureSize.bottom,				// 출력 대상의 사각형 높이
		(m_origin_p.Xsal + m_move_p.Xsal),	// DIB 원본의 좌상귀 x 좌표
		(m_origin_p.Ysal + m_move_p.Ysal),	// DIB 원본의 좌상귀 y 좌표
		m_Resize.Xsal,						// DIB 원본 사각형 너비
		m_Resize.Ysal,						// DIB 원본 사각형 높이
		m_Image.data,						// 픽셀 데이터 시작 주소
		m_BitMapInfo,						// BITMAPINFO 구조체 시작 주소
		DIB_RGB_COLORS,						// RGB 또는 색상 테이블 인덱스
		SRCCOPY								// 래스터 연산 코드
	);

	PicDC.BitBlt(0, 0, m_PictureSize.right, m_PictureSize.bottom, &m_memDC, 0, 0, SRCCOPY);

	if (m_Roiflag & TRUE) { DisplayROI(); }
	
	//imshow("내부데이터", m_Image);

	//dynamic allocation free && Release DC
	::ReleaseDC(m_wnd->m_hWnd, PicDC);
	m_memDC.DeleteDC();
	m_Bmp.DeleteObject();
	delete[](BYTE*)m_BitMapInfo;
}

void DrawWnd::SetRoiPoint(CPoint start, CPoint end)
{
	m_Roiflag = TRUE;

	//원점 복귀용 좌표
	m_strRoi.Xsal = start.x;
	m_strRoi.Ysal = start.y;
	m_endRoi.Xsal = end.x;
	m_endRoi.Ysal = end.y;

	//확대가 되던 축소가 되던 원본 이미지 좌표로 변환됨
	ConvertPictureToImage(m_strRoi, originalstrROI);
	ConvertPictureToImage(m_endRoi, originalendROI);
	m_originStartpRoi = originalstrROI;
	m_originEndRoi	= originalendROI;
	if (m_ImageRead & TRUE) { Display(); }
}

void DrawWnd::DisplayROI()
{
	CClientDC PicDC(m_wnd);
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));

	//원하는 영역에만 그림 그리기
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_PictureSize);
	PicDC.SelectClipRgn(&rgn);

	//속이 빈 사각형 그리기
	PicDC.SelectObject(&pen);

	SelectObject(PicDC, GetStockObject(NULL_BRUSH));

	PicDC.Rectangle
	(
		static_cast<int>(m_strRoi.Xsal), static_cast<int>(m_strRoi.Ysal),
		static_cast<int>(m_endRoi.Xsal), static_cast<int>(m_endRoi.Ysal)
	);

	//사용 완료된 DC 릴리즈
	::ReleaseDC(m_wnd->m_hWnd, PicDC);
	pen.DeleteObject();
}

void DrawWnd::ChangeImageType(cv::Mat& image, ImageType eImage, InspData sInspData)
{
	if (image.empty()) { return; }

	switch (sInspData.eImageType)
	{
	case eImage_Color:
		break;

	case eImage_Gray:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
		}
		break;

	case eImage_Binary:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
		}
		else
		{
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);

		}
		break;

	case eImage_Edge:
		if (image.elemSize() * 8 != 8)
		{
			cvtColor(image, image, COLOR_BGR2GRAY);
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
			Canny(image, image, sInspData.EdgeLOW, sInspData.EdgeHigh);
		}
		else
		{
			threshold(image, image, sInspData.ThresHold, 255, THRESH_BINARY);
			Canny(image, image, sInspData.EdgeLOW, sInspData.EdgeHigh);
		}
		break;
	}
}

void DrawWnd::SetImageSize()
{
	//원본이미지 사이즈
	m_Size.Xsal = m_Image.cols;
	m_Size.Ysal = m_Image.rows;

	//리사이즈 이미지 사이즈
	m_Resize = m_Size;
	//m_Resize.Xsal = m_Size.Xsal;
	//m_Resize.Ysal = m_Size.Ysal;

	ConvertImageToPicture(originalstrROI, m_strRoi);
	ConvertImageToPicture(originalendROI, m_endRoi);
}

void DrawWnd::createBIT()
{
	int bpp = 8 * m_Image.elemSize(); // 픽셀

	// bmp 파일 헤더정보 설정
	m_BitMapInfo->bmiHeader.biBitCount = bpp;
	m_BitMapInfo->bmiHeader.biWidth = m_Image.cols;
	m_BitMapInfo->bmiHeader.biHeight = -m_Image.rows;
	m_BitMapInfo->bmiHeader.biPlanes = 1;
	m_BitMapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BitMapInfo->bmiHeader.biCompression = BI_RGB;
	m_BitMapInfo->bmiHeader.biClrImportant = 0;
	m_BitMapInfo->bmiHeader.biClrUsed = 0;
	m_BitMapInfo->bmiHeader.biSizeImage = 0;
	m_BitMapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_BitMapInfo->bmiHeader.biYPelsPerMeter = 0;

	// gray scale palette
	if (bpp == 8)
	{
		m_RGB = m_BitMapInfo->bmiColors;
		for (int i = 0; i < dByte; i++)
		{
			m_RGB[i].rgbRed	  =	(BYTE)i;
			m_RGB[i].rgbGreen = (BYTE)i;
			m_RGB[i].rgbBlue  =	(BYTE)i;
			m_RGB[i].rgbReserved = 0;
		}
	}
}

void DrawWnd::Zoom(short zDelta, CPoint point)
{
	float ZoomScale = 0.5;
	if (zDelta > 0) 
	{
		m_fScale += ZoomScale;
	}
	else if (zDelta < 0) 
	{
		m_fScale -= ZoomScale;
	}

	if (m_fScale <= 0.5f)
	{
		m_fScale = 0.5f;
	}

	resizeLength();
	ConvertImageToPicture(originalstrROI, m_strRoi);
	ConvertImageToPicture(originalendROI, m_endRoi);
	if (m_ImageRead & TRUE) { Display(); }
}

void DrawWnd::move(CPoint Back_p, CPoint New_p)
{
	resizeLength();
	float MoveOffset = 0.5; //움직이는 거리가 너무 민감해서 가중치를 곱하여 움직이는 폭을 줄여준다.
	m_movelenght.Xsal = static_cast<float>((New_p.x - Back_p.x)* MoveOffset);
	m_movelenght.Ysal = static_cast<float>((New_p.y - Back_p.y)* MoveOffset);

	m_move_p.Xsal -= (m_movelenght.Xsal * m_Pixel.Xsal);
	m_move_p.Ysal += (m_movelenght.Ysal * m_Pixel.Ysal);

	ConvertImageToPicture(originalstrROI, m_strRoi);
	ConvertImageToPicture(originalendROI, m_endRoi);
	if (m_ImageRead & TRUE) { Display(); }
}

void DrawWnd::ChangeRoiflag(BOOL state)
{
	m_Roiflag = state;
	if (m_ImageRead & TRUE) { Display(); }
}

void DrawWnd::GetImageToPicturePixel()
{
	resizeLength();
	m_Pixel.Xsal = m_Resize.Xsal / static_cast<float>(m_PictureSize.right);
	m_Pixel.Ysal = m_Resize.Ysal / static_cast<float>(m_PictureSize.bottom);
}

void DrawWnd::resizeLength()
{
	m_Resize.Xsal = m_Size.Xsal / m_fScale;
	m_Resize.Ysal = m_Size.Ysal / m_fScale;
	m_origin_p.Xsal = ((m_Size.Xsal - m_Resize.Xsal) / 2.f);
	m_origin_p.Ysal = ((m_Size.Ysal - m_Resize.Ysal) / 2.f);
	m_offset.Xsal = ((m_Size.Xsal - m_Resize.Xsal) / 2.f);
	m_offset.Ysal = ((m_Size.Ysal - m_Resize.Ysal) / 2.f);
}

void DrawWnd::ConvertImageToPicture(Scale Image, Scale& Picture)
{
	GetImageToPicturePixel();
	if (m_fScale != 1.0)
	{
		Image.Xsal -= m_offset.Xsal;
		Image.Ysal -= m_offset.Ysal;
	}
	Picture.Xsal = (Image.Xsal - m_move_p.Xsal) / m_Pixel.Xsal;
	Picture.Ysal = (Image.Ysal + m_move_p.Ysal) / m_Pixel.Ysal;
}

void DrawWnd::ConvertPictureToImage(Scale Picture, Scale& Image)
{
	GetImageToPicturePixel();
	Image.Xsal = (Picture.Xsal * m_Pixel.Xsal) + m_move_p.Xsal;
	Image.Ysal = (Picture.Ysal * m_Pixel.Ysal) - m_move_p.Ysal;

	if (m_fScale != 1.0)
	{
		Image.Xsal += m_offset.Xsal;
		Image.Ysal += m_offset.Ysal;
	}
}

//void DrawWnd::GetEdge(int TH,int pos)
//{
//
//	THimg = m_cloneimg.clone();
//	Mat t = m_cloneimg.clone();
//	if (pos == 0) 
//	{
//		for (int j = originstrROI.Ysal; j <= originendROI.Ysal; j++)
//		{
//			for (int i = originstrROI.Xsal; i <= originendROI.Xsal; i ++)
//			{
//				t.at<BYTE>(j, i) = (m_cloneimg.at<BYTE>(j, i) <= TH) ? 255 : 0;
//			}
//		}
//	}
//	else if(pos == 1)
//	{
//		for (int j = originstrROI.Ysal; j <= originendROI.Ysal; j++)
//		{
//			for (int i = originstrROI.Xsal; i <= originendROI.Xsal; i++)
//			{
//				t.at<BYTE>(j, i) = (m_cloneimg.at<BYTE>(j, i) <= TH) ? 0 : 255;
//			}
//		}
//	}
//	
//	int VctX, VctY;
//	double grandiant;
//	for (int y = originstrROI.Ysal+1; y < originendROI.Ysal-1; y++)
//	{
//		for (int x = originstrROI.Xsal+1; x < originendROI.Xsal-1; x++)
//		{
//			VctX = (-t.at<BYTE>(y - 1, x - 1)) + (0 * t.at<BYTE>(y - 1, x)) + (t.at<BYTE>(y - 1, x + 1))
//				+ (-2 * t.at<BYTE>(y, x - 1)) + (0 * t.at<BYTE>(y, x)) + (2 * t.at<BYTE>(y, x + 1))
//				+ (-t.at<BYTE>(y + 1, x - 1)) + (0 * t.at<BYTE>(y + 1, x)) + (t.at<BYTE>(y + 1, x + 1));
//
//			VctY = (-t.at<BYTE>(y - 1, x - 1)) + (-2 * t.at<BYTE>(y - 1, x)) + (-1 * t.at<BYTE>(y - 1, x + 1))
//				+ (0 * t.at<BYTE>(y, x - 1)) + (0 * t.at<BYTE>(y, x)) + (0 * t.at<BYTE>(y, x + 1))
//				+ (t.at<BYTE>(y + 1, x - 1)) + (2 * t.at<BYTE>(y + 1, x)) + (t.at<BYTE>(y + 1, x + 1));
//
//			grandiant = sqrt((VctX*VctX) + (VctY*VctY));
//			THimg.at<BYTE>(x - 1, y - 1) = static_cast<BYTE>(limit(grandiant + 0.5));
//			/*if (static_cast<BYTE>(limit(grandiant + 0.5)) >= 255) 
//			{
//				PicDC.MoveTo(x - 1, y - 1);
//				PicDC.LineTo(x - 1, y 
//				- 1);
//			}*/
//		}
//	}
//	       
//	Display();
//}

void DrawWnd::ConvertImageToPicture(CPoint Image, CPoint& Picture)
{
	GetImageToPicturePixel();
	if (m_fScale != 1.0)
	{
		Image.x -= m_offset.Xsal;
		Image.y -= m_offset.Ysal;
	}
	Picture.x = (Image.x - m_move_p.Xsal) / m_Pixel.Xsal;
	Picture.y = (Image.y + m_move_p.Ysal) / m_Pixel.Ysal;
}

void DrawWnd::ConvertPictureToImage(CPoint Picture, CPoint& Image)
{
	GetImageToPicturePixel();
	Image.x = (Picture.x * m_Pixel.Xsal) + m_move_p.Xsal;
	Image.y = (Picture.y * m_Pixel.Ysal) - m_move_p.Ysal;
	if (m_fScale != 1.0)
	{
		Image.x += m_offset.Xsal;
		Image.y += m_offset.Ysal;
	}
}

void DrawWnd::SetRetore()
{
	//이미지 원점
	m_origin_p.Xsal = 0;
	m_origin_p.Ysal = 0;
	m_move_p.Xsal = 0;
	m_move_p.Ysal = 0;

	m_movelenght.Xsal = 0;
	m_movelenght.Ysal = 0;
	
	//원본이미지 사이즈
	m_Size.Xsal = m_Image.cols;
	m_Size.Ysal = m_Image.rows;
	
	//리사이즈 이미지 사이즈
	m_Resize.Xsal = m_Size.Xsal;
	m_Resize.Ysal = m_Size.Ysal;

	//스케일 초기화
	m_fScale = 1.0f;

	ConvertImageToPicture(originalstrROI, m_strRoi);
	ConvertImageToPicture(originalendROI, m_endRoi);
	if (m_ImageRead & TRUE) { Display(); }
}

void DrawWnd::DrawRoi(CPoint start, CPoint End)
{
	CClientDC PicDC(m_wnd);
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));

	//원하는 영역에만 그림 그리기
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_PictureSize);
	PicDC.SelectClipRgn(&rgn);

	//속이 빈 사각형 그리기
	PicDC.SelectObject(&pen);

	SelectObject(PicDC, GetStockObject(NULL_BRUSH));

	ConvertImageToPicture(start, start);
	ConvertImageToPicture(End, End);

	//m_wnd->ScreenToClient(&start);
	//m_wnd->ScreenToClient(&End);

	PicDC.Rectangle
	(
		static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(End.x), static_cast<int>(End.y)
	);

	//사용 완료된 DC 릴리즈
	::ReleaseDC(m_wnd->m_hWnd, PicDC);
	pen.DeleteObject();
}