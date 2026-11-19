// DifferentConfig.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	#ifdef _MyOwnConfig
		std::cout<<"\nYou are building MyOwnConfig build of your application\n";
	#else
		std::cout<<"\nThis is certainly not MyOwnConfig build\n";
	#endif

	std::cout<<"\nThis part of the code would build no matter what build you are using\n";
	return 0;
}

