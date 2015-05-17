#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "CLStatus.h"

using namespace std;

#ifndef _CLLOGGER_H_
#define _CLLOGGER_H_

class CLLogger
{
public:
	static CLLogger* GetInstance();
	static CLStatus WriteLogMsg(const char* pstrMsg, long lErrorCode);
	CLStatus WriteLog(const char* pstrMsg, long lErrorCode);
	CLStatus flush();

private:
	CLLogger(const CLLogger& s);
	CLLogger& operator=(const CLLogger& s);

	CLLogger();
	~CLLogger();

	int m_fd;
	static CLLogger* m_pLog;

private:
	char* m_pLogBuffer;
	unsigned int m_nUsedBytesForBuffer;
};

#endif