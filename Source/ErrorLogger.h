#pragma once

#include <iostream>

class Logger
{
public:
	Logger();

	void LogErrorsWarnings(int flag);

private:
	int m_flag = 0;
};