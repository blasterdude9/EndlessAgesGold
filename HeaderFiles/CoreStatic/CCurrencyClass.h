#ifndef CCURRENCYCLASS_H
#define CCURRENCYCLASS_H

class  CCurrencyObj : public CObject
{
	
     
	DECLARE_SERIAL( CCurrencyObj );
	
    
public:
    CCurrencyObj();
    
	CString m_currencyName;
	long    m_amount;
  
	void Clone(CCurrencyObj ** clone);
	void Serialize( CArchive& ar );	
	void SerializeAlloc( CArchive& ar );
};

#endif