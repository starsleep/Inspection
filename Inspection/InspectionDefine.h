#pragma once

#define SAFE_DELETE(p) if(nullptr != p) {delete p ; p = nullptr;}

//사용자 메시지
#define UM_LIVE					(WM_USER + 100)
#define UM_LIVE_FIT				(WM_USER + 101)
#define UM_ZOOM_IN				(WM_USER + 102)
#define UM_ZOOM_OUT				(WM_USER + 103)
#define UM_CoonetCam			(WM_USER + 104)
#define UM_initialization		(WM_USER + 105)
#define UM_UPDATE_RESULT		(WM_USER + 106)
#define UM_RECIPE_UPDATE		(WM_USER + 107)
#define UM_UPDATE_LLIST			(WM_USER + 108)
#define UM_DISPLAY_CAM			(WM_USER + 109)
#define UM_DISPLAY_CAM			(WM_USER + 110)
#define UM_DISPLAY_CAM			(WM_USER + 111)
#define UM_DISPLAY_CAM			(WM_USER + 112)

#define INSPECTION_PATH		_T("C:\\SWINSPECTION\\RECIPE")
#define IMAGESAVE_PATH		_T("C:\\SWINSPECTION\\IMAGESAVE")
#define TEMPLATEIMAGESAVE_PATH		_T("C:\\SWINSPECTION\\TEMPLATESAVE")

template <typename T>
class TemplateSingleton
{
protected:
	TemplateSingleton() {}
	virtual ~TemplateSingleton() {};

private:
	static T* m_pInstance;

public:
	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T;
			atexit(DestroyInstance);
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance != nullptr)
		{
			SAFE_DELETE(m_pInstance);
		}
	
	}
};

enum InspButton
{
	eCam_Connet = 0,
	eCam_Live,
	eCam_Stop,
	eCam_StratInsp,
	eCam_Capture,
	eCam_ZoomIn,
	eCam_Zoomout,
	eCam_ZoomFit,
	eCam_Recipe,
	eCam_Count
};

enum ImageType
{
	eImage_Color,
	eImage_Gray,
	eImage_Binary,
	eImage_Edge,
	eImage_Count
};

enum eresult
{
	ePass,
	eNonPass,
	result_Count
};

enum resultdata
{
	ePassstate,
	ePreCent,
	eCount,
	resultdata_Count
};

typedef struct InspData
{
	int topX;
	int topY;
	int BotX;
	int BotY;
	int ThresHold;
	int EdgeHigh;
	int EdgeLOW;
	int Standard;
	int Upper;
	int Lower;
	int eImageType;

	InspData& operator=(const InspData& rhs)
	{
		this->topX = rhs.topX;
		this->topY = rhs.topY;
		this->BotX = rhs.BotX;
		this->BotY = rhs.BotY;
		this->ThresHold = rhs.ThresHold;
		this->EdgeHigh = rhs.EdgeHigh;
		this->EdgeLOW = rhs.EdgeLOW;
		this->Standard = rhs.Standard;
		this->Upper = rhs.Upper;
		this->Lower = rhs.Lower;
		this->eImageType = rhs.eImageType;
		return *this;
	}
}InspData;