#ifndef C3DMENUCLASS_H
#define C3DMENUCLASS_H
  
class  C3DMenuClass : public CObject
{
 public:
 
 C3DMenuClass();
 ~C3DMenuClass();
 
 void AddMenuObjectSkeletal(CString objectName, CString fileName, D3DVECTOR position);
 void AddMenuObjectMesh(CString objectName, CString fileName, D3DVECTOR position, LPDIRECT3D7 pD3D7);
 void AddMenuObjectText(CString objectName, CString text, D3DVECTOR position);
 void AddCursor(CString texName, int texIndex, int numBlocks, int blockSize );
 void SetCurrentMenu(int currMenu);
 void SafeDelete();
 
 CString m_menuName;
 int m_menuID;

 CObList m_cursorsList;
 CObList m_menuObjList;
};
  
#endif