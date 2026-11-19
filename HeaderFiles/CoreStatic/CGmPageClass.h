#ifndef CGMPAGECLASS_H
#define CGMPAGECLASS_H

class  CGmPageObj : public CObject
{
public:
    CGmPageObj();
    
	int m_handle; 
};

class  CGmPageList : public CObList
{
public:
   CGmPageList(){};
   void SafeDelete();
   void RemovePage(int handle);
   void AddPage(int handle);
};

#endif