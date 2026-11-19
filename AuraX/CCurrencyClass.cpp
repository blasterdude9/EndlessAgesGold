#include "stdafx.h"
#include <afxtempl.h>
#include "CCurrencyClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCurrencyObj::CCurrencyObj()
{
   m_currencyName = "CREDITS";
   m_amount       = 0;
}

void CCurrencyObj::Clone(CCurrencyObj ** clone)
{
     CCurrencyObj * copyLocal = new CCurrencyObj();
	 copyLocal->m_amount         = m_amount;
	 copyLocal->m_currencyName   = m_currencyName;

	 *clone = copyLocal;
}

void CCurrencyObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_currencyName << m_amount;
	      
	}
	else
	{
       ar >> m_currencyName >> m_amount;
	      
	}
}     

void CCurrencyObj::SerializeAlloc( CArchive& ar )
{
	if(ar.IsStoring() ){	
	   ar << m_currencyName << m_amount;
	      
	}
	else
	{
       ar >> m_currencyName >> m_amount;
	      
	}
}     

IMPLEMENT_SERIAL(CCurrencyObj, CObject, 0)


