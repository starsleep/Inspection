#pragma once

#include "opencv2/opencv.hpp"
#include "InspectionDefine.h"
#include <iostream>
#include <vector>

#define dByte 256

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

typedef struct Scale
{
	float Xsal = 0.f;
	float Ysal = 0.f;

	Scale& operator=( const Scale& rhs) 
	{
		this->Xsal = rhs.Xsal;
		this->Ysal = rhs.Ysal;
		return *this;
	}

}Scale;

class DrawWnd
{
private:

	//picture control DC
	CWnd*				m_wnd;			//Picture 
	RECT				m_PictureSize;	//Picture Size 
	BITMAPINFO*			m_BitMapInfo;
	RGBQUAD*			m_RGB;
	CDC					m_memDC;
	CBitmap*			m_OldBmp;
	CBitmap				m_Bmp;
	cv::Mat				m_Image;
	cv::Mat				m_BackUp;
	Scale				m_Size; //원본 이미지 사이즈
	Scale				m_Resize; // 조절된 이미지 사이즈
	BOOL				m_ImageRead;
	//픽처 컨트롤 1칸당 원본이지미 픽셀의 수
	Scale				m_Pixel; // 원본 이미지/ 픽처 컨트롤 

	Scale				 m_origin_p; //이미지의 시작점
	Scale				 m_offset; //오프셋;
	Scale				 m_move_p;
	
	//Move mouse
	CPoint				m_Back_p;	// 이전 마우스 좌표
	CPoint				m_New_p;	// 현재 마우스 좌표
	Scale				m_movelenght;	// 마우스가 움직인 거리

	float				m_fScale; // Zoom in /out 배율 (확대 축소 배율)

	// ROI
	Scale				m_originStartpRoi;	//원본 좌표(불변)
	Scale				m_originEndRoi;	//원본 좌표(불변)

	Scale				m_strRoi; //ROI좌표픽처컨트롤 기준(가변)
	Scale				m_endRoi; //ROI좌표픽처컨트롤 기준(가변)

	Scale				originalstrROI; //원본 ROI 좌표(픽처컨트롤 기준)
	Scale				originalendROI; //원본 ROI 좌표(픽처컨트롤 기준)

	BOOL				m_Roiflag;
	BOOL				m_zoomflag;
	BOOL				m_THFlag;

	

public:

	DrawWnd();

	~DrawWnd();

	BOOL initialization(CWnd* Wnd);

	void GetWnd(CWnd* wnd);

	void SetImageSize();

	void SetImage(cv::Mat& image, ImageType eImage, InspData sInspData);

	BOOL ImageRead(cv::Mat& img, ImageType eImagetype, InspData sInspData,cv::Mat& BackUp);

	void ChangeRoiflag(BOOL state); // ROI 그리는 모드이거나 아니거나

	void createBIT(); //비트맵 헤더 만드는 함수

	void Display(); //픽처컨트롤에 디스플레이 해주는 함수

	void SetRetore(); // 파라미터 원복

	void GetSize(); //픽처컨트롤의 윈도우 사이즈 

	void Zoom(short zDelta, CPoint point); //줌인 줌아웃

	void move(CPoint Back_p, CPoint New_p); //마우스 움직인 위치 계산

	void SetRoiPoint(CPoint start, CPoint end); // 드래그 시작 위치 , 끝나는 위치 세팅

	void DisplayROI(); // ROI 디스플레이

	void DrawRoi(CPoint start , CPoint End);

	void GetImageToPicturePixel();

	void resizeLength();

	void ConvertImageToPicture(Scale Image, Scale& Picture);

	void ConvertPictureToImage(Scale Picture, Scale& Image);

	//void GetEdge(int TH ,int pos);

	void ChangeImageType(cv::Mat& image, ImageType eImage, InspData sInspData);

	void ConvertImageToPicture(CPoint Image, CPoint& Picture);

	void ConvertPictureToImage(CPoint Picture, CPoint& Image);

	void SetImageRread(BOOL bState) { m_ImageRead = bState; }
	
};

