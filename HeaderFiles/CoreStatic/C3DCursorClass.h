#ifndef C3DCURSORCLASS_H
#define C3DCURSORCLASS_H

#define TEXTURESIZE 256

class  C3DCursorClass : public CObject
{
	public:

	C3DCursorClass();
	~C3DCursorClass();
	
	RECT m_constraints;

	DWORD m_lastTick;
	WORD m_indecies[6];
	
	D3DVERTEX m_verts[4];
 	D3DVECTOR m_position;
	D3DVECTOR m_speed;

	CString m_name;
	CString m_textureFileName;

	BOOL m_isVisible;
	BOOL m_isCursorAnimated;
	BOOL m_isLocked;

	float m_texDistance;

	int m_textureIndex;
	int m_blendMode;
	int m_blockCnt;
	int m_blockSize;
	int m_blocksPerRow;
	int m_currBlockNum;
	int m_currBlockX;
	int m_currBlockY;
	int m_msBetweenAnimations;
    int m_cursorID;
	
	float m_clickPointX;
	float m_clickPointY;
	float m_absMousePosX, m_absMousePosY;
	float m_left, m_right, m_top, m_bottom;
	float m_cursorSpeed;

	//drag drop vars
	BOOL m_dragging;
	int  m_dragGlid;
	

	void InitTexture
		(
		CString textureName,
		int textureIndex,
		int numBlocks,
		int blockSize,
		BOOL isCursorAnimated,
		int msBetweenAnimations
		);
	
	//vertClickHotSpot { -1 = middle, 0 = tl, 1 = tr, 2 = br, 3 = bl }
	void MakeAMesh(float size, int blendMode, int vertClickHotSpot, float cursorSpeed);
	void Render(LPDIRECT3DDEVICE7 pd3dDevice7);
	void Constrain(float left, float right, float top, float bottom);
	void Lock();
	void Unlock();
	 
};

#endif
