#pragma once
#include <string>
#include <iostream>

using namespace std;

class Exception
{
public:
	string message;

	Exception() {}
	

	Exception(string _msg)
	{
		message = _msg;
	}

	~Exception() {}
	
};


