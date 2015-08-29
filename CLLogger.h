#include "Common.h"
#include "CLStatus.h"

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
	static void OnProcessExit();
	static pthread_mutex_t* InitMutex();
	CLStatus WriteMsgAndErrcodeToFile(const char* pstrMsg,const char* pstrErrcode);
	CLLogger(const CLLogger& s);
	CLLogger& operator=(const CLLogger& s);

	CLLogger();
	~CLLogger();


private:
	int m_fd;
	static CLLogger* m_pLog;
	char* m_pLogBuffer;
	unsigned int m_nUsedBytesForBuffer;
	bool m_bFlagForProcessExit;
	pthread_mutex_t* m_pMutexForWriteLog;
	static pthread_mutex_t* m_pMutexForCreateLog;

};

#endif
