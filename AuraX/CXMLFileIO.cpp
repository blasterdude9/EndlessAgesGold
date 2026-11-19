#include "stdafx.h"
#include "CXMLFileIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXMLFileIO::CXMLFileIO()
{
	m_file = NULL;
	m_curSpacing[0] = '\0';
}

CXMLFileIO::~CXMLFileIO()
{
	Close();
}

bool CXMLFileIO::Open(char *file, bool storing)
{
	//Figure out if we're reading or writing
	char mode[]	= "r";
	m_isStoring	= false;

	if(storing)
	{
		mode[0] = 'w';
		m_isStoring = true;
	}

	//Open the file stream
	m_file = fopen(file, mode);

	if(m_file == NULL)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	Peek()
// Description:	Returns a bool value indicating if the next tag is an OPEN or
//				CLOSE tag. Does not move the current position of the stream
//				indicator.
///////////////////////////////////////////////////////////////////////////////

bool 
CXMLFileIO::Peek()
{
	static char buffer[MAX_BUFFER];
	bool closing;
	fpos_t position;

	fgetpos (this->m_file, &position);
	ReadTag(buffer, MAX_BUFFER, closing);
	fsetpos (this->m_file, &position);

	return closing;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	PeekTag(char*)
// Description:	File the input parameter with the name of the next tag. Does
//				not move the current position of the stream indicator.
///////////////////////////////////////////////////////////////////////////////

void 
CXMLFileIO::PeekTag(char *buffer)
{
	bool closing;
	fpos_t position;

	fgetpos (this->m_file, &position);
	ReadTag(buffer, MAX_BUFFER, closing);
	fsetpos (this->m_file, &position);
}

void CXMLFileIO::WriteOpeningTag(char *tag)
{
	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Write out the tag
	fwrite("<", 1, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">\n", 2, 1, m_file);

	//Increase the spacing
	int i = 0;
	while(m_curSpacing[i] != '\0')
		i++;
	m_curSpacing[i]		= '\t';
	m_curSpacing[i + 1] = '\0';
}

void CXMLFileIO::WriteAttribute(char *tag, char *attribute)
{
	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Open the tag
	fwrite("<", 1, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">", 1, 1, m_file);

	//Write out the attribute
	fwrite(attribute, strlen(attribute), 1, m_file);

	//Close the tag
	fwrite("</", 2, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">\n", 2, 1, m_file);
}

void CXMLFileIO::WriteAttribute(char *tag, int attribute)
{
	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Open the tag
	fwrite("<", 1, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">", 1, 1, m_file);

	//Write out the attribute
	char buffer[20];
	itoa(attribute, buffer, 10);
	fwrite(buffer, strlen(buffer), 1, m_file);

	//Close the tag
	fwrite("</", 2, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">\n", 2, 1, m_file);
}

void CXMLFileIO::WriteAttribute(char *tag, bool attribute)
{
	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Open the tag
	fwrite("<", 1, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">", 1, 1, m_file);

	//Write out the attribute
	if(attribute)
		fwrite("true", 4, 1, m_file);
	else
		fwrite("false", 5, 1, m_file);

	//Close the tag
	fwrite("</", 2, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">\n", 2, 1, m_file);
}

void CXMLFileIO::WriteMultilinedAttribute(char *attribute)
{
	//Write out the attribute(line by line)
	int curStart	 = 0;
	int curLength	 = 0;
	int stringLength = strlen(attribute);
	for(int i = 0; i < stringLength; i++)
	{
		curLength++;
		if(attribute[i] == '\n')
		{
			//Write out the spacing
			fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

			//Write out the current line
			fwrite(attribute + curStart, curLength, 1, m_file);

			//Reset the tracing values
			curStart = i + 1;
			curLength = 0;
		}
	}
}

void CXMLFileIO::WriteLineOfAttribute(char *attribute)
{
	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Write out the line of the attribute
	fwrite(attribute, strlen(attribute), 1, m_file);

	//Write out a new line
	fwrite("\n", 1, 1, m_file);
}

void CXMLFileIO::WriteClosingTag(char *tag)
{
	//Decrease the spacing
	int i = 1;
	while(m_curSpacing[i] != '\0')
		i++;
	m_curSpacing[i - 1]	= '\0';

	//Write out the spacing
	fwrite(m_curSpacing, strlen(m_curSpacing), 1, m_file);

	//Write out the tag
	fwrite("</", 2, 1, m_file);
	fwrite(tag, strlen(tag), 1, m_file);
	fwrite(">\n", 2, 1, m_file);
}

void CXMLFileIO::ReadTag(char *buffer, int maxSize, bool &closing)
{
	//Read 1 line
	fgets(buffer, maxSize, m_file);

	//Find the beginning of the tag
	int start = 0;
	while(buffer[start] != '<')
		start++;

	//See if the tag is closing
	if(buffer[++start] == '/')
	{
		start++;
		closing = true;
	}
	else
		closing = false;

	//Copy the tag to the beginning of the buffer
	int beginning = 0;
	while(buffer[start] != '>')
		buffer[beginning++] = buffer[start++];

	//Put the null terminator on the end
	buffer[beginning] = '\0';
}

void CXMLFileIO::ReadAttribute(char *attribute, int maxSize)
{
	//Read 1 line
	fgets(attribute, maxSize, m_file);

	//Find the beginning of the attribute
	int start = 1;
	while(attribute[start - 1] != '>')
		start++;

	//Copy the attribute to the beginning of the buffer
	int beginning = 0;
	while(attribute[start] != '<')
		attribute[beginning++] = attribute[start++];

	//Put the null terminator on the end
	attribute[beginning] = '\0';
}

void CXMLFileIO::ReadAttribute(float &attribute)
{
	//Read 1 line
	char buffer[MAX_BUFFER];
	fgets(buffer, MAX_BUFFER, m_file);

	//Find the beginning of the attribute
	int start = 1;
	while(buffer[start - 1] != '>')
		start++;

	//Copy the attribute to the beginning of the buffer
	int beginning = 0;
	while(buffer[start] != '<')
		buffer[beginning++] = buffer[start++];

	//Put the null terminator on the end
	buffer[beginning] = '\0';

	//Convert the attribute to an integer
	attribute = atof(buffer);
}

float CXMLFileIO::ReadAttribute()
{
	//Read 1 line
	char buffer[MAX_BUFFER];
	fgets(buffer, MAX_BUFFER, m_file);

	//Find the beginning of the attribute
	int start = 1;
	while(buffer[start - 1] != '>')
		start++;

	//Copy the attribute to the beginning of the buffer
	int beginning = 0;
	while(buffer[start] != '<')
		buffer[beginning++] = buffer[start++];

	//Put the null terminator on the end
	buffer[beginning] = '\0';

	//Convert the attribute to an integer
	float attribute = atof(buffer);
	return attribute;
}

void CXMLFileIO::ReadAttribute(int &attribute)
{
	//Read 1 line
	char buffer[MAX_BUFFER];
	fgets(buffer, MAX_BUFFER, m_file);

	//Find the beginning of the attribute
	int start = 1;
	while(buffer[start - 1] != '>')
	{
		start++;
		if(start > MAX_BUFFER) 
			return;
	}

	//Copy the attribute to the beginning of the buffer
	int beginning = 0;
	while(buffer[start] != '<')
	{
		buffer[beginning++] = buffer[start++];
		if(start > MAX_BUFFER) 
			break;
	}

	//Put the null terminator on the end
	buffer[beginning] = '\0';

	//Convert the attribute to an integer
	attribute = atoi(buffer);
}

void CXMLFileIO::ReadAttribute(bool &attribute)
{
	//Read 1 line
	char buffer[MAX_BUFFER];
	fgets(buffer, MAX_BUFFER, m_file);

	//Find the beginning of the attribute
	int start = 1;
	while(buffer[start - 1] != '>')
		start++;

	//See if the attribute is true or false
	if(buffer[start] == 't' || buffer[start] == 'T')
		attribute = true;
	else
		attribute = false;
}

void CXMLFileIO::ReadLineOfAttribute(char* attribute, int maxSize)
{
	//Read 1 line
	fgets(attribute, maxSize, m_file);

	//Find the start of the attribute
	int start = 0;
	while(attribute[start] == '\t' || attribute[start] == ' ')
		start++;

	//Copy it to the beginning of the buffer
	int beginning = 0;
	int stringLength = strlen(attribute);
	while(start < stringLength)
		attribute[beginning++] = attribute[start++];

	attribute[beginning] = '\0';
}