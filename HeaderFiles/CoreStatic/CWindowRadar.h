#pragma once

class CWindowRadar
{
public:
	CWindowRadar() { m_menu = NULL; }

	//Visual Components
	CMenuWindow	*m_menu;
	CMenuText	*m_region;
	CMenuImage	*m_map;
	CMenuImage	*m_compass;
	CMenuText	*m_coordinateX;
	CMenuText	*m_coordinateY;
	CMenuText	*m_zoomRadius;
	CMenuImage	*m_groupDot;
	CMenuImage	*m_playerDot;
	CMenuImage	*m_npcDot;
	CMenuImage	*m_mobDot;

	//Settings
	float m_mapScale;	//Minimap versus Real Map Ratio
	float m_curZoom;	//Current zoom radius in meters
	float m_zoomScale;	//Zoomed Minimap versus Real Map Ratio

	//Stored starting position for dots
	POSITION m_dotStart;
};