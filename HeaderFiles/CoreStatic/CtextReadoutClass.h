
#include "CTextRenderClass.h"
#include "CStateManagementClass.h"

#ifndef CTEXTREADOUT_H
#define CTEXTREADOUT_H

class CStringObj : public CObject
{
	
     
    DECLARE_SERIAL( CStringObj )
    
    
public:
	
	CStringObj();
    CString      m_text;
	CTextureRenderObj * m_textVisual;
	float m_red;
	float m_green;
	float m_blue;

	void        SafeDelete();
	void        Clone(CStringObj ** copy);
	void        Serialize( CArchive& ar );

};

class CStringObjList : public CObList
{
public:
   CStringObjList(){};
   void Clone(CStringObjList ** copy);
   void SafeDelete();
    
     
    DECLARE_SERIAL( CStringObjList )
    
    
};

//text readout class
class CtextReadoutObj : public CObject
{
	
     
    DECLARE_SERIAL( CtextReadoutObj )
    
    
public:
	
	CtextReadoutObj();
    
    int m_vertSpacing;
	int m_historyCount;
	int m_maxLineLength;
	int m_verticleLineVistCount;
	int m_timeOption;
	int m_locationX;//10000X10000
	int m_locationY;//10000X10000
	float m_cgLocationX;
    float m_cgLocationY;
	float m_cgVertSpacing;
	int   m_cgBlendMode;
	float m_cgFontSizeX;
	float m_cgFontSizeY;
	int m_currentOffset;
	long m_scrollStamp;
	float m_maxMeasuredLength;
	bool m_reinit;
	int m_speechMode;
	CSymbolMapperObjList * m_symbolMapSysRef;

	//Chronicle Colors Chris Hack
	float m_normalTextRed;
	float m_normalTextGreen;
	float m_normalTextBlue;
	
	float m_clanTextRed;
	float m_clanTextGreen;
	float m_clanTextBlue;

	float m_guildTextRed;
	float m_guildTextGreen;
	float m_guildTextBlue;

	float m_serverTextRed;
	float m_serverTextGreen;
	float m_serverTextBlue;

	CStringObjList * m_textLines;

	void        AddLine(float floatNumber);
	void        RenderTextCG(CStateManagementObj * statemanager,LPDIRECT3DDEVICE9 g_pd3dDevice);
	CStringObj  *GetHeadVisual();
	void        ReinitVisualSection(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode);
	void        SafeDelete();
	void        AddLine(CString textLine,
						float red,
					    float green,
					    float blue);
	void        Clone(CtextReadoutObj ** copy);
	CString     FloatToStingCnv(float number);
	void        AddLine(D3DVECTOR vector);
	void        ScrollDown(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode);
	void        ScrollUp(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode);
	void        AddLine(CString text);
	void        AddLine(AXDVECTOR vector);
	void        AddLineChina(CString textLine,
							 float red,
							 float green,
							 float blue);
	void        AddLine(CString label,AXDVECTOR vector);
	void		AddLine(CString label,D3DVECTOR vector);
	void        AddLine(CString label,float floatNumber);
	void        Serialize( CArchive& ar );
	void        Flush();

};

class CtextReadoutObjList : public CObList
{
public:
   CtextReadoutObjList(){};
   void Clone(CtextReadoutObjList ** copy);
   void SafeDelete();
    
     
    DECLARE_SERIAL( CtextReadoutObjList )
    
    
};


#endif