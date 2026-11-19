
#ifndef DECLARE_SERIAL_EXTDLL
#define DECLARE_SERIAL_EXTDLL(class_name) _DECLARE_DYNCREATE(class_name) AFX_EXT_API friend CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb);
#endif