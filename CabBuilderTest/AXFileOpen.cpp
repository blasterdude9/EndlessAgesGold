// AXFileOpen.cpp : implementation file
//

#include "stdafx.h"
#include "direct.h"
#include "CabBuilderTest.h"
#include "AXFileOpen.h"
#include ".\axfileopen.h"


// CAXFileOpen dialog

IMPLEMENT_DYNAMIC(CAXFileOpen, CDialog)
CAXFileOpen::CAXFileOpen(CWnd* pParent /*=NULL*/)
	: CDialog(CAXFileOpen::IDD, pParent)
{
}

CAXFileOpen::~CAXFileOpen()
{

   if(m_fileInfo)
      delete [] m_fileInfo;
   m_fileInfo = 0;


}

void CAXFileOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_folderView, m_folderList);
	DDX_Control(pDX, IDC_Combo_HotLinks, m_comboHotLinks);
	DDX_Control(pDX, IDC_BUTTON_UpAlevel, m_upLevelBtnHandle);
}


BEGIN_MESSAGE_MAP(CAXFileOpen, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_folderView, OnNMDblclkListfolderview)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UpAlevel, OnBnClickedButtonUpalevel)
	ON_CBN_SELCHANGE(IDC_Combo_HotLinks, OnCbnSelchangeComboHotlinks)
	ON_BN_CLICKED(IDC_BUTTON_upDirLevel, OnBnClickedButtonupdirlevel)
END_MESSAGE_MAP()


// CAXFileOpen message handlers

BOOL CAXFileOpen::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	m_hotDirCount = 0;

	m_fileInfo = NULL;
	m_fileInfoCount = 0;

	

    m_imageList = new CImageList();
	//*m_gBufferCount = 0;
   
	HIMAGELIST himageListTemp = GetSystemImageListHandle( TRUE ,&m_globalHanldeImgssi);
	m_imageList->Attach(himageListTemp);
	m_folderList.SetImageList(m_imageList,LVSIL_SMALL);
	m_comboHotLinks.SetImageList(m_imageList);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_upLevelBtnHandle.SetIcon(hIcon);
	
    
	
	m_folderList.InsertColumn(0,"Name",LVCFMT_LEFT,200,0);
	m_folderList.InsertColumn(1,"Size",LVCFMT_RIGHT,100,0);
	m_folderList.InsertColumn(2,"Type",LVCFMT_LEFT,100,0);
	
   
	//m_currentFolder = "C:\\Documents and Settings\\Arb\\My Documents\\";
	char buffer[_MAX_PATH];
	 _getcwd( buffer, _MAX_PATH );
	CString CurDir = buffer;
	m_currentFolder = CurDir+"\\";//GetDeskTopFolder();
 
	InListFolder(m_currentFolder, -1);
	
	//*m_glbuffer = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAXFileOpen::AddComboItem(int index, 
							   int image, 
                               CString str,
							   int indentCount,
							   CComboBoxEx * comboBox)
{
COMBOBOXEXITEM     cbi;
int                nItem;

cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
     CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

cbi.iItem = index;
cbi.pszText = (LPTSTR)(LPCTSTR)str;
cbi.cchTextMax = str.GetLength();
cbi.iImage = image;
cbi.iSelectedImage = image;
cbi.iOverlay = 2;
cbi.iIndent = (indentCount);   //Set indentation according
                            //to item position

nItem = comboBox->InsertItem(&cbi);
}


void CAXFileOpen::InListDirHotLinks(CString currentFolder,int sel)
{
     m_comboHotLinks.ResetContent();

	 
	 m_hotDirCount = 0;

	 TCHAR g_szDrvMsg[] = _T("\tA:");
     ULONG uDriveMask = _getdrives();

	 int insertTrace = 0;
	 //m_comboHotLinks.AddString("Desktop");
	 //m_comboHotLinks.InsertItem(
	 //deskTop key
	 AddComboItem(insertTrace++,
		          GetDirIconIndex(true ),//int image, 
                  "Desktop",//CString str,
				  0,
				  &m_comboHotLinks);
    
	 m_hotList[m_hotDirCount++] = GetDeskTopFolder();

	 //parse currentFolder
	 CString temp = currentFolder;
	 int entryCount = 0;
	 int indentTree = 0;
	 int step = temp.GetLength()-1;
	 CString treeTemp[50];
	 CString treeTempDir[50];
	 int treeCount = 0;
	 while(step > -1)
	 {
         CString subBuild;
		 int travers = 0;
		 while(temp.GetAt(step) != '\\')
		 {
		       subBuild = temp.GetAt(step--)+subBuild;
               if(step < 0)
			      break;  
			   travers++;
		 }
		 if(travers == 0)
		 {
		 travers++;
		 step--;
		 }

		 if(subBuild.GetLength() > 0)
		 {
		 treeTemp[entryCount] = subBuild;
		 treeTempDir[entryCount] = temp + "\\";
		 
		 
         entryCount++;
		 }
         //delete section
		
            temp.Delete(temp.GetLength()-travers,travers);
	 }
	 //insertTrace += entryCount;
	 //fill in data
     int indentTrace = 1;
     for(int loop = entryCount-1;loop>-1;loop--)
	 {
	  sel = insertTrace;
	 AddComboItem(insertTrace++,
		             GetDirIconIndex(true ),//int image, 
                     treeTemp[loop],//CString str,
				     indentTrace++,
				     &m_comboHotLinks);
	 m_hotList[m_hotDirCount++] = treeTempDir[loop];
	 }
     



     if(uDriveMask > 0)
	 {
      while (uDriveMask) {
         if (uDriveMask & 1)
		 {
             CString tempFormat = g_szDrvMsg;
			 tempFormat.Replace("\t","");
			 //m_comboHotLinks.AddString("   "+tempFormat);
			 AddComboItem(insertTrace++,
		          GetDirIconIndex(true ),//int image, 
                  tempFormat,//CString str,
				  1,
				  &m_comboHotLinks);
			 m_hotList[m_hotDirCount++] = tempFormat + "\\";
		 }
            //printf(g_szDrvMsg);

         ++g_szDrvMsg[1];
         uDriveMask >>= 1;
      }
	 }

     if(sel > -1)
	 {
	    m_comboHotLinks.SetCurSel(sel);
        m_comboHotLinks.SetTopIndex(sel);
		
	 }

}

void CAXFileOpen::InListFolder(CString rootFolder, int sel)
{
	 WIN32_FIND_DATA FindData;
     int trace = 0;
	 DWORD Attrib;
	 m_folderList.DeleteAllItems();
	 //FOLDERS
	 CString sPath = rootFolder+"*.*";
	 HANDLE FileHandle = FindFirstFile(sPath, &FindData);
	 if(FileHandle == INVALID_HANDLE_VALUE)
	    return;
        
	 

     do
	 {
         Attrib = FindData.dwFileAttributes;

		 CString type;
		 int imageType = 0;
	     if(Attrib == 16)
		 {//folder
            type = "Folder";
            if(FindData.cFileName[0] == '.')
			   continue;

		 }//end folder
		 else if(Attrib == 17)
		 {//folder
            continue;


		 }//end folder
		 else
		 {//file
		    continue;
            type = "File";
            imageType = 1;
		 }//end file
		 
		 CString size = "0 KB";
         if(FindData.nFileSizeLow == 0)
		 {
		 }
		 else if(FindData.nFileSizeLow < 1000)
		 {
            size = "1 KB";
		 }
		 else if(FindData.nFileSizeLow < 1000000)
		 {
            size = ConvertLongToString(FindData.nFileSizeLow/1000) + " KB";
		 }
		 else
		 {
            size = ConvertLongToString(FindData.nFileSizeLow/1000000) + " MB";
		 }

         
		
		 m_folderList.InsertItem(trace,FindData.cFileName,GetDirIconIndex( TRUE ));
		 m_folderList.SetItemText(trace,1,size);
		 m_folderList.SetItemText(trace,2,type);
		 
         trace++;
		 
	 }
	 while(FindNextFile(FileHandle, &FindData));

     //FILES
     FileHandle = FindFirstFile(sPath, &FindData);
     do
	 {
         Attrib = FindData.dwFileAttributes;

		 CString type;
		 int imageType = 0;
	     if(Attrib == 16)
		 {//folder
            type = "Folder";
            continue;

		 }//end folder
		 else if(Attrib == 17)
		 {//folder
            continue;


		 }//end folder
		 else
		 {//file
            type = "File";
            imageType = 1;
		 }//end file
		 
		 CString size = "0 KB";
         if(FindData.nFileSizeLow == 0)
		 {
		 }
		 else if(FindData.nFileSizeLow < 1000)
		 {
            size = "1 KB";
		 }
		 else if(FindData.nFileSizeLow < 1000000)
		 {
            size = ConvertLongToString(FindData.nFileSizeLow/1000) + " KB";
		 }
		 else
		 {
            size = ConvertLongToString(FindData.nFileSizeLow/1000000) + " MB";
		 }

         
		
		 m_folderList.InsertItem(trace,FindData.cFileName,GetFileIconIndex( FindData.cFileName , TRUE ));
		 m_folderList.SetItemText(trace,1,size);
		 m_folderList.SetItemText(trace,2,type);
		 
         trace++;
		 
	 }
	 while(FindNextFile(FileHandle, &FindData));



	 if(sel > -1)
	 {
	    m_folderList.SetItemState(sel,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED );
		m_folderList.EnsureVisible(sel,FALSE);
	 }
	 InListDirHotLinks(rootFolder,0);

}

CString CAXFileOpen::ConvertLongToString(long number)
{    
	          if(number==0)
				 return "0";

			  char buffer[67];

			  _ltoa(number,buffer,10);
		
			  return buffer;
}






void CAXFileOpen::OnBnClickedOk()
{
	m_fileInfo = NULL;
	m_fileInfoCount = 0;
	DWORD totalByteSize = 0;
    int selCount = m_folderList.GetSelectedCount();
	int totalTitemCount = m_folderList.GetItemCount();
    int trace = 0;
	//SIZE LOOP
	int loop=0;
	for(loop=0;loop<totalTitemCount;loop++)
	{
		if(m_folderList.GetItemState(loop,LVIS_SELECTED)==LVIS_SELECTED)
		{
		   if(m_folderList.GetItemText(loop,2).CompareNoCase("Folder")==0)
		   {
              CString folderTemp = m_currentFolder + m_folderList.GetItemText(loop,0) + "\\";
			  GetFileCount(folderTemp, m_fileInfoCount, totalByteSize);
		   }
		   else
		   {
		      CString fileTemp = m_currentFolder + m_folderList.GetItemText(loop,0);
			  m_fileInfoCount++;
		   }

           trace++;
		}
	}
	m_fileInfo = new S_fileInfo[m_fileInfoCount];
	m_fileInfoCount = 0;
	//FILL LOOP
    for(loop=0;loop<totalTitemCount;loop++)
	{
		if(m_folderList.GetItemState(loop,LVIS_SELECTED)==LVIS_SELECTED)
		{
		   if(m_folderList.GetItemText(loop,2).CompareNoCase("Folder")==0)
		   {
              CString folderTemp = m_currentFolder + m_folderList.GetItemText(loop,0) + "\\";
			  FillFileInfo(folderTemp, m_fileInfoCount);
		   }
		   else
		   {
		      CString fileTemp = m_currentFolder + m_folderList.GetItemText(loop,0);
			  m_fileInfo[m_fileInfoCount].s_filePath = fileTemp;

			  WIN32_FIND_DATA FindData;
              HANDLE FileHandle = FindFirstFile(fileTemp, &FindData);
			  if(FileHandle == INVALID_HANDLE_VALUE)
			     m_fileInfo[m_fileInfoCount++].s_fileSize = 0;
			  else
			     m_fileInfo[m_fileInfoCount++].s_fileSize = FindData.nFileSizeLow;
		   }

           trace++;
		}
	}

	OnOK();
}

void CAXFileOpen::OnNMDblclkListfolderview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    
		CString text = m_folderList.GetItemText(
							       pNMLV->iItem,//int nItem,
							       0);
		CString type = m_folderList.GetItemText(
							       pNMLV->iItem,//int nItem,
							       2);

        if(type.CompareNoCase("Folder") == 0)
		{
		  m_currentFolder += text+"\\";
		  SetCurrentDirectory(m_currentFolder);
          InListFolder(m_currentFolder, -1);
		}
		else
		{//single file add
           OnBnClickedOk();
		}//end single file add
	
	*pResult = 0;
}

int CAXFileOpen::GetFileIconIndex( CString strFileName , BOOL bSmallIcon )
{
	SHFILEINFO    sfi;
	
	if (bSmallIcon)
	{
        SHGetFileInfo(
           (LPCTSTR)strFileName, 
           FILE_ATTRIBUTE_NORMAL,
           &sfi, 
           sizeof(SHFILEINFO), 
           SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		SHGetFileInfo(
           (LPCTSTR)strFileName, 
           FILE_ATTRIBUTE_NORMAL,
           &sfi, 
           sizeof(SHFILEINFO), 
           SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
	int index = sfi.iIcon;


    return index;
	
}

int CAXFileOpen::GetDirIconIndex(BOOL bSmallIcon )
{
	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Doesn't matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		 SHGetFileInfo(
		 (LPCTSTR)"Doesn't matter", 
		 FILE_ATTRIBUTE_DIRECTORY,
		 &sfi, 
		 sizeof(SHFILEINFO), 
		 SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);

	}
    return sfi.iIcon;

}

HIMAGELIST  CAXFileOpen::GetSystemImageListHandle( BOOL bSmallIcon ,SHFILEINFO * ssfiRet )
{
	HIMAGELIST  hSystemImageList; 
	//SHFILEINFO    ssfi; 

   if (bSmallIcon)
   {
	hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
    (LPCTSTR)_T("c:\\"), 
     0, 
     ssfiRet, 
     sizeof(SHFILEINFO), 
     SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 
   }
   else
   {
		hSystemImageList = 
    (HIMAGELIST)SHGetFileInfo( 
    (LPCTSTR)_T("c:\\"), 
     0, 
     ssfiRet, 
     sizeof(SHFILEINFO), 
     SHGFI_SYSICONINDEX | SHGFI_LARGEICON); 
   }
   //ssfiRet = &ssfi;
   return hSystemImageList;
}

CString CAXFileOpen::GetDeskTopFolder()
{
    IShellFolder * sFolder = NULL;
	HRESULT err = SHGetDesktopFolder(&sFolder);
    STRRET name;
	ITEMIDLIST listID;
    err = sFolder->GetDisplayNameOf(NULL,
		                            SHGDN_FORPARSING,//DWORD uFlags,
                                    &name);//LPSTRRET lpName

	/*char charArray[MAX_PATH];
	ZeroMemory(charArray,MAX_PATH);
	int length = GetLPWSTRLength(name.pOleStr);
	CopyMemory(charArray,name.pOleStr,length);*/
	CString folderName = ConvertLPWSTR(name.pOleStr);
	folderName += "\\";
    //delete sFolder;
	//sFolder = 0;
    return folderName;
}

CString CAXFileOpen::ConvertLPWSTR(LPWSTR & string)
{
	CString temp;
    int trace = 0;
	while(1)
	{
	   if(string[trace] == 0)
          break;

	   temp += string[trace];
	   //trace+=2;
	   trace++;

	}
    return temp;
}


void CAXFileOpen::OnDestroy()
{
	CDialog::OnDestroy();

	m_folderList.SetImageList(NULL, LVSIL_SMALL);
	m_comboHotLinks.SetImageList(NULL);

	m_imageList->Detach();
	delete m_imageList;
	m_imageList = 0;

	

}

HICON CAXFileOpen::GetFileIconHandle(CString strFileName, BOOL bSmallIcon)
{
	
	SHFILEINFO    sfi;
	if (bSmallIcon)
	{
		SHGetFileInfo(
		   (LPCTSTR)strFileName, 
		   FILE_ATTRIBUTE_NORMAL,
		   &sfi, 
		   sizeof(SHFILEINFO), 
		   SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	}
	else
	{
		SHGetFileInfo(
		   (LPCTSTR)strFileName, 
		   FILE_ATTRIBUTE_NORMAL,
		   &sfi, 
		   sizeof(SHFILEINFO), 
		   SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
	}
	return sfi.hIcon;


}

void CAXFileOpen::OnBnClickedButtonUpalevel()
{
	
}

void CAXFileOpen::OnCbnSelchangeComboHotlinks()
{
	int sel = m_comboHotLinks.GetCurSel();
	if(sel < 0 || sel > m_hotDirCount)
	   return;
	
    SetCurrentDirectory(m_hotList[sel]);
	m_currentFolder = m_hotList[sel];
	InListFolder(m_hotList[sel], -1);
}

void CAXFileOpen::OnBnClickedButtonupdirlevel()
{
	int size = m_currentFolder.GetLength()-1;
	CString potentialNewFolder = m_currentFolder;
	int slashCount = 0;
    for(int loop=size;loop>0;loop--)
	{
        if(potentialNewFolder.GetAt(loop)=='\\')
		{
           slashCount++;
           if(slashCount == 2)
		      break;
		}
		potentialNewFolder.Delete(loop,1);
	}
	if(slashCount == 2)
	{
       m_currentFolder = potentialNewFolder;
       SetCurrentDirectory(m_currentFolder);
	   InListFolder(m_currentFolder, -1);
	}
	
}


int CAXFileOpen::FillFileInfo(CString path, int & currentCount)
{
	 WIN32_FIND_DATA FindData;
	 DWORD Attrib;
	 CString sPath = path+"*.*";
     HANDLE FileHandle = FindFirstFile(sPath, &FindData);
	 if(FileHandle == INVALID_HANDLE_VALUE)
	    return 0;
     do
	 {
         Attrib = FindData.dwFileAttributes;

		 CString type;
		 int imageType = 0;
	     if(Attrib == 16)
		 {//folder
            
			if(FindData.cFileName[0] == '.')
			   continue;
			FillFileInfo((path+FindData.cFileName)+"\\", currentCount);


		 }//end folder
		 else if(Attrib == 17)
		 {//folder
            continue;
		 }//end folder
		 else
		 {//file
            m_fileInfo[currentCount].s_filePath = path+FindData.cFileName;
			m_fileInfo[currentCount].s_fileSize = FindData.nFileSizeLow;
            currentCount++;
		 }//end file
		 		 
	 }
	 while(FindNextFile(FileHandle, &FindData));
	 return currentCount;
}

int CAXFileOpen::GetFileCount(CString path, int & currentCount, DWORD & byteSize)
{
	 WIN32_FIND_DATA FindData;
	 DWORD Attrib;
	 CString sPath = path+"*.*";
     HANDLE FileHandle = FindFirstFile(sPath, &FindData);
	 if(FileHandle == INVALID_HANDLE_VALUE)
	    return 0;
     do
	 {
         Attrib = FindData.dwFileAttributes;

		 CString type;
		 int imageType = 0;
	     if(Attrib == 16)
		 {//folder
            
			if(FindData.cFileName[0] == '.')
			   continue;
			GetFileCount((path+FindData.cFileName)+"\\", currentCount,byteSize);


		 }//end folder
		 else if(Attrib == 17)
		 {//folder
            continue;
		 }//end folder
		 else
		 {//file
            byteSize += FindData.nFileSizeLow;
            currentCount++;
		 }//end file
		 		 
	 }
	 while(FindNextFile(FileHandle, &FindData));
	 return currentCount;
}



