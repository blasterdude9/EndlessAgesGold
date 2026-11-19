//LOGON context packet 
typedef struct {
	signed char m_catagorie;
	char  username[25];
	char  password[25];
	short versionInfo;
} CONTEXT_VALIDATION, *LPCONTEXT_VALIDATION;