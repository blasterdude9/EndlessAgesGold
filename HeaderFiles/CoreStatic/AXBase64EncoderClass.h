#pragma once

#include    <stdlib.h>                      // calloc and free prototypes.
#include    <stdio.h>                       // printf prototype.
#include    <string.h>                      // str* and memset prototypes.

typedef
unsigned
char    uchar;                              // Define unsigned char as uchar.

typedef
unsigned
int     uint;                               // Define unsigned int as uint.

class CAXBase64EncoderClass
{
public:
	CAXBase64EncoderClass(void){};
	~CAXBase64EncoderClass(void){};

	

    //Functions
	bool b64decode(char *s);
	bool b64encode(char *s);
	void b64stats(char *s, char *b, bool f);
	char * b64buffer(char *s, bool f);
	bool  b64valid(uchar *c);
	bool b64isnot(char *p, char *b);
};
