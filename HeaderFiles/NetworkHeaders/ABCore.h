#include <stdlib.h>
#include <stdio.h>



class ABString
{
     public:
     ABString(){m_data=NULL;m_length=0;}
	 ~ABString(){if(m_data)delete[]m_data;m_data=0;m_length=0;}
	 char * m_data;
	 int m_length;

	 int GetLength(){return m_length;};
	 char GetAt(int index)
	 {
		  if(index < m_length)return m_data[index];
		  else return ' ';
	 }
	 bool GetAt(int index, char * retChar)
	 {
		  if(index < m_length)
		  {
			  *retChar = m_data[index];
			  return true;
		  }
		  else return false;
	 }


     // operator overloads
	 

	 ABString & operator=( char * charArray )
	 {
         if(!charArray)
            return *this;

         if(m_data)delete [] m_data;m_data=0;m_length=0;

		 int trace = 0;
		 while(charArray[trace] != 0)
		    trace++;
		 
		 
		 if(trace > 0)
		 {
	        m_length = trace;
		    m_data = new char[m_length+1];
		    CopyMemory(m_data,charArray,m_length);
			m_data[m_length] = 0;
		 }		    
		 return *this;
	 }

	 ABString & operator+=( ABString & temp )
	 {
		 if(temp.m_length > 0)
		 {
		    int newLength = temp.m_length + m_length;
            char * newString = new char[newLength+1];
			newString[newLength] = 0;

			if(m_length>0)CopyMemory(newString,m_data,m_length);
			
			CopyMemory(&newString[m_length],temp.m_data,temp.m_length);

			*this = newString;
			delete [] newString;
			newString=0;
			
			return *this;
		 }		    
		 return *this;
	 }



	 ABString operator+( ABString & temp )
	 {
		 ABString * newABStr = new ABString();
		 if(temp.m_length > 0)
		 {
		    int newLength = temp.m_length + m_length;
            char * newString = new char[newLength+1];
			newString[newLength] = 0;

			if(m_length>0)CopyMemory(newString,m_data,m_length);
			
			CopyMemory(&newString[m_length],temp.m_data,temp.m_length);

			*newABStr = newString;
			delete [] newString;
			newString=0;
			
			return *newABStr;
		 }		    
		 return *newABStr;
	 }

	 ABString operator+( char * charArray )
	 {
		 ABString * newABStr = new ABString();
         int newStringLength = 0;
		 while(charArray[newStringLength] != 0)
		      newStringLength++;

		 if(newStringLength > 0)
		 {
		    int newLength = newStringLength + m_length;

            char * newString = new char[newLength+1];
			newString[newLength] = 0;

			if(m_length>0)CopyMemory(newString,m_data,m_length);
			
			CopyMemory(&newString[m_length],charArray,newStringLength);

			*newABStr = newString;
			delete [] newString;
			newString=0;
			
			return *newABStr;
		 }		    
		 return *newABStr;
	 }

	 friend ABString operator+( char * charArray, ABString & abStr)
	 {
         ABString * newABStr = new ABString();

         int newStringLength = 0;
		 while(charArray[newStringLength] != 0)
		      newStringLength++;
       
		 if(newStringLength > 0)
		 {
		    int newLength = newStringLength + abStr.m_length;

            char * newString = new char[newLength+1];
			newString[newLength] = 0;

            CopyMemory(&newString[0],charArray,newStringLength);

			if(abStr.m_length>0)CopyMemory(&newString[newStringLength],abStr.m_data,abStr.m_length);
			
			

			*newABStr = newString;
			delete [] newString;
			newString=0;
			
			return *newABStr;
		 }		    
		 return *newABStr;
	 }

	 ABString & operator=( ABString & temp )
	 {
         
		 *this = temp.m_data;
		 
		  return *this;
	 }

     ABString & operator=( int number )
	 {
	          if(number==0)
			  {
				 *this = "0";
                 return *this;
			  }
	 
	          int decimal,sign;
              char *buffer; 
	          buffer = _fcvt(number,0,&decimal,&sign);
			  
			  if(sign != 0)
			  {
			     int newStringLength = 0;
		         while(buffer[newStringLength] != 0)
		               newStringLength++;
				 newStringLength++;

				 if(m_data)delete[]m_data;m_data=0;
				 m_length = newStringLength;
 
				 m_data = new char[newStringLength+1];
				 m_data[newStringLength] = 0;
                 m_data[0] = '-';
                 if(newStringLength > 0)
				    CopyMemory(&m_data[1],buffer,newStringLength-1);
			  }
			  else
			     *this = buffer;
		
			  return *this;
	 }
};

//2 byte floating point variable
class ABFloat
{
   public:
   ABFloat(){m_cVal=0;}
   short m_cVal;

   float GetFloat(){return ((float)m_cVal)*.0001f;}
   ABFloat & operator=( float temp ){m_cVal = (short)(temp*10000.0f);return *this;}
};

/*class ARBVect
{
   public:
   ARBVect(){};
   ARBFloat x,y,z;
   short GetXcompressed(){return x.m_cVal;}
   short GetYcompressed(){return y.m_cVal;}
   short GetZcompressed(){return z.m_cVal;}
   float GetX(){return x.GetFloat();}
   float GetY(){return y.GetFloat();}
   float GetZ(){return z.GetFloat();}
   D3DVECTOR GetD3DVECTOR(){D3DVECTOR temp; 
                            temp.x = x.GetFloat();
							temp.y = y.GetFloat();
							temp.z = z.GetFloat();
							return temp;}

   mARBVect & operator=( D3DVECTOR temp ){x = temp.x;y = temp.y;z = temp.z;return *this;}
};*/