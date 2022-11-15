#pragma once
#include "DrawWnd.h"
#include <Vector>

class InspectionThread : public CWinThread
{
	DECLARE_DYNCREATE(InspectionThread)

private:
	BOOL				m_bThUse;
	CRITICAL_SECTION	m_csInsp;
	InspData			m_InspData;
	int					result[result_Count];
	cv::Mat				m_Template;

	public:

		virtual BOOL InitInstance();

		virtual int Exitlnstance();

		virtual int Run();

		void SetThreadState(BOOL use);

		void ReSetCount();

		BOOL GetTemplate(cv::Mat& Templ);
};

