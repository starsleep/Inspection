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
	Scale				m_Size; //���� �̹��� ������
	Scale				m_Resize; // ������ �̹��� ������
	BOOL				m_ImageRead;
	//��ó ��Ʈ�� 1ĭ�� ���������� �ȼ��� ��
	Scale				m_Pixel; // ���� �̹���/ ��ó ��Ʈ�� 

	Scale				 m_origin_p; //�̹����� ������
	Scale				 m_offset; //������;
	Scale				 m_move_p;
	
	//Move mouse
	CPoint				m_Back_p;	// ���� ���콺 ��ǥ
	CPoint				m_New_p;	// ���� ���콺 ��ǥ
	Scale				m_movelenght;	// ���콺�� ������ �Ÿ�

	float				m_fScale; // Zoom in /out ���� (Ȯ�� ��� ����)

	// ROI
	Scale				m_originStartpRoi;	//���� ��ǥ(�Һ�)
	Scale				m_originEndRoi;	//���� ��ǥ(�Һ�)

	Scale				m_strRoi; //ROI��ǥ��ó��Ʈ�� ����(����)
	Scale				m_endRoi; //ROI��ǥ��ó��Ʈ�� ����(����)

	Scale				originalstrROI; //���� ROI ��ǥ(��ó��Ʈ�� ����)
	Scale				originalendROI; //���� ROI ��ǥ(��ó��Ʈ�� ����)

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

	void ChangeRoiflag(BOOL state); // ROI �׸��� ����̰ų� �ƴϰų�

	void createBIT(); //��Ʈ�� ��� ����� �Լ�

	void Display(); //��ó��Ʈ�ѿ� ���÷��� ���ִ� �Լ�

	void SetRetore(); // �Ķ���� ����

	void GetSize(); //��ó��Ʈ���� ������ ������ 

	void Zoom(short zDelta, CPoint point); //���� �ܾƿ�

	void move(CPoint Back_p, CPoint New_p); //���콺 ������ ��ġ ���

	void SetRoiPoint(CPoint start, CPoint end); // �巡�� ���� ��ġ , ������ ��ġ ����

	void DisplayROI(); // ROI ���÷���

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

