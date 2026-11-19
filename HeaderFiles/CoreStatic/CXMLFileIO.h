#pragma once

#include <stdio.h>

#define MAX_BUFFER			256

class CXMLFileIO
{
public:
	CXMLFileIO();
    ~CXMLFileIO();
	//Base File IO Functions
	bool Open(char *file, bool storing = false);
	void Close() { if(m_file)fclose(m_file); }
	bool Peek();
	void PeekTag(char *buffer);

	//Writing
	void WriteOpeningTag(char *tag);
	void WriteAttribute(char *tag, char *attribute);
	void WriteAttribute(char *tag, int attribute);
	void WriteAttribute(char *tag, bool attribute);
	void WriteMultilinedAttribute(char *attribute);
	void WriteLineOfAttribute(char *attribute);
	void WriteClosingTag(char *tag);

	//Reading
	//TODO: Create a ReadTag that fseeks to the next line
	void ReadTag(char *buffer, int maxSize, bool &closing);
	void ReadAttribute(char *attribute, int maxSize);
	void ReadAttribute(float &attribute);
	void ReadAttribute(int &attribute);
	void ReadAttribute(bool &attribute);
	void ReadLineOfAttribute(char* attribute, int maxSize);
	float ReadAttribute();

	FILE *m_file;
	char  m_curSpacing[MAX_BUFFER];
	bool  m_isStoring;
};