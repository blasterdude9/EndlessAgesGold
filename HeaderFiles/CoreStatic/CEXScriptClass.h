#ifndef CEXSCRIPTCLASS_H
#define CEXSCRIPTCLASS_H

//scripting class
//main use - for auto loading initial Databases
//Definitions Attributes
//0 = play movie fullscreen
//1 = open scene
//2 = load missile database
//3 = load explosion database
//4 = load movie database
//5 = load controls database
//6 = load default cameras
//7 = load anim textures database
//8 = load mouse database
//9 = load 2D collections database
//10 = load text/font collections database
//11 = load bullethole Database

class  CEXScriptObj : public CObject
{
	CEXScriptObj(){}
	
    
   DECLARE_SERIAL( CEXScriptObj )
   
   
public:
	
	CEXScriptObj(int         L_actionAttribute,
		         CString     L_miscString,
		         int         L_miscInt
		  	     );

    int         List_actionAttribute;
    CString     List_miscString;
    int         List_miscInt;    
    
	void Serialize( CArchive& ar );

};

class  CEXScriptObjList : public CObList
{
public:
   CEXScriptObjList(){};
   
   void SafeDelete();
   DECLARE_SERIAL( CEXScriptObjList )
   
   
};

#endif