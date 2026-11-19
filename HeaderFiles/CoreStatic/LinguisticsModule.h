#pragma once
#include "core.h"

class AFX_EXT_CLASS CLinguisticsModule
{
public:
	CLinguisticsModule(CString dataPath);
	~CLinguisticsModule(void);

	//Variables
	Core * m_botBrainCore;
	char m_fromUserText[2048];
    char m_botResponse[2048];
	PUvm pu;
	//functions
	CString SpeechHandler(CString fromUserText);
};
