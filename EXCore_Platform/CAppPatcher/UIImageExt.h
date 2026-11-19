#pragma once

class CUIImageExt
{
public:
	CUIImageExt(void);
public:
	~CUIImageExt(void);


   HBITMAP LoadAnImage(char* FileName, int resHandle)
   {
   // Use IPicture stuff to use JPG / GIF files
   IPicture* p;
   IStream* s;
   //IPersistStream* ps;
   HGLOBAL hG;
   void* pp;
   FILE* fp;

   if(resHandle < 0)
   {//file based
   // Read file in memory
   fp = fopen(FileName,"rb");
   if (!fp)
   return NULL;

   fseek(fp,0,SEEK_END);
   int fs = ftell(fp);
   fseek(fp,0,SEEK_SET);
   hG = GlobalAlloc(GPTR,fs);
   if (!hG)
      {
      fclose(fp);
      return NULL;
      }
   pp = (void*)hG;
   fread(pp,1,fs,fp);
   fclose(fp);
   CreateStreamOnHGlobal(hG,false,&s);
   if (!s)
      {
      GlobalFree(hG);
      return NULL;
      }

   OleLoadPicture(s,0,false,IID_IPicture,(void**)&p);

   if (!p)
      {
      s->Release();
      GlobalFree(hG);
      return NULL;
      }
   s->Release();
   GlobalFree(hG);

   HBITMAP hB = 0;
   p->get_Handle((unsigned int*)&hB);

   // Copy the image. Necessary, because upon p's release,
   // the handle is destroyed.
   HBITMAP hBB = (HBITMAP)CopyImage(hB,IMAGE_BITMAP,0,0,
                                    LR_COPYRETURNORG);

   p->Release();
   return hBB;
   }//end file based
   else
   {//resource based
	   HRSRC res = FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(resHandle),"JPEG");
	   
	   if (res) 
	   {
		   HGLOBAL mem = LoadResource(GetModuleHandle(NULL), res);
           void * data = LockResource(mem);
           size_t sz = SizeofResource(GetModuleHandle(NULL), res);

		   hG = GlobalAlloc(GPTR,sz);
           CopyMemory(hG,data,sz);
		  
		   CreateStreamOnHGlobal(hG,false,&s);
		   if (!s)
			  {
			  GlobalFree(hG);
			  return NULL;
			  }

		   OleLoadPicture(s,0,false,IID_IPicture,(void**)&p);

		   if (!p)
			  {
			  s->Release();
			  GlobalFree(hG);
			  return NULL;
			  }
		   s->Release();
		   GlobalFree(hG);

		   HBITMAP hB = 0;
		   p->get_Handle((unsigned int*)&hB);

		   // Copy the image. Necessary, because upon p's release,
		   // the handle is destroyed.
		   HBITMAP hBB = (HBITMAP)CopyImage(hB,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

		   p->Release();
		   return hBB;
	   }
   }//end resource based
   // Create an IStream so IPicture can
   return 0;
   }
};
