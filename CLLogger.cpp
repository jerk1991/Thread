#include "CLLogger.h"

#define LOG_FILE_NAME "CLLogger.text"
#define MAX_SIZE 256
#define BUFFER_SIZE_LOG_FILE 4096

CLLogger* CLLogger::m_pLog = NULL;
pthread_mutex_t* CLLogger::m_pMutexForCreateLog = CLLogger::InitMutex();

CLLogger::CLLogger() {
	m_fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if(m_fd == -1)
		throw "In CLLogger::CLLogger(), open error";

	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	m_nUsedBytesForBuffer = 0;
	m_bFlagForProcessExit = false;
	m_pMutexForWriteLog = new pthread_mutex_t;
	if(pthread_mutex_init(m_pMutexForWriteLog,0) != 0) {
		delete m_pMutexForWriteLog;
		delete [] m_pLogBuffer;
		close(m_fd);
		throw "In CLLogger::CLLogger(), pthread_mutex_init error";
	}
}

CLLogger::~CLLogger() {
	if(m_fd != -1) 
		close(m_fd);
	delete m_pMutexForWriteLog;
	delete [] m_pLogBuffer;
}

CLLogger::CLLogger(const CLLogger& s) {
	
	m_fd = s.m_fd;
	m_pLog = s.m_pLog;
	m_bFlagForProcessExit = s.m_bFlagForProcessExit;
	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	strcpy(m_pLogBuffer, s.m_pLogBuffer);
	m_nUsedBytesForBuffer = s.m_nUsedBytesForBuffer;
	m_pMutexForWriteLog = new pthread_mutex_t;
	if(pthread_mutex_init(m_pMutexForWriteLog,0) != 0) {
		delete m_pMutexForWriteLog;
		delete [] m_pLogBuffer;
		throw "In CLLogger::CLLogger(const CLLogger&),copy error";
	}
}

CLLogger& CLLogger::operator=(const CLLogger& s) {
	
	if(this == &s) return *this;
	
	delete [] m_pLogBuffer;
	m_pLogBuffer = NULL;
	
	m_fd = s.m_fd;
	m_pLog = s.m_pLog;
	m_bFlagForProcessExit = s.m_bFlagForProcessExit;
	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	strcpy(m_pLogBuffer, s.m_pLogBuffer);
	m_nUsedBytesForBuffer = s.m_nUsedBytesForBuffer;

	m_pMutexForWriteLog = new pthread_mutex_t;
	if(pthread_mutex_init(m_pMutexForWriteLog,0) != 0) {
		delete m_pMutexForWriteLog;
		delete [] m_pLogBuffer;
		throw "In CLLogger::operator=(const CLLogger&),pass value error";
	}

	return *this; 
}

CLLogger* CLLogger::GetInstance() {
	
	if(m_pLog != 0)
		return m_pLog;
	if(m_pMutexForCreateLog == 0)
		return NULL;
	if(pthread_mutex_lock(m_pMutexForCreateLog) != 0)
		return NULL;

	if(m_pLog == 0) {
		try {
			m_pLog = new CLLogger;
		}
		catch(const char*) {
			pthread_mutex_unlock(m_pMutexForCreateLog);
			return 0;
		}
		if(atexit(CLLogger::OnProcessExit) != 0) {
			if(pthread_mutex_lock(m_pLog->m_pMutexForWriteLog) != 0)
				return 0;
			m_pLog->m_bFlagForProcessExit = true;
			if(pthread_mutex_unlock(m_pMutexForCreateLog) != 0)
				return 0;
			if(pthread_mutex_unlock(m_pLog->m_pMutexForWriteLog) != 0)
				return 0;
			m_pLog->WriteLog("In CLLogger::GetInstance(), atexit error", errno);
		}
		return m_pLog;
	}

	if(pthread_mutex_unlock(m_pMutexForCreateLog) != 0)
		return NULL;
		
	return m_pLog;
}

CLStatus CLLogger::WriteLog(const char* pstrMsg, long lErrorCode) {
	if(pstrMsg == NULL) return CLStatus(-1,0);

	if(strlen(pstrMsg) == 0) return CLStatus(-1,0);

	if(m_fd == -1) return CLStatus(-1,0);

	unsigned int len_strmsg = strlen(pstrMsg);

	char buf[MAX_SIZE];
	snprintf(buf, MAX_SIZE, "Error code: %ld\r\n", lErrorCode);
	unsigned int len_code = strlen(buf);

	unsigned int total_len = len_strmsg + len_code;

	if(pthread_mutex_lock(m_pMutexForWriteLog) != 0) 
		return CLStatus(-1,0);
	try	{

		if((total_len > BUFFER_SIZE_LOG_FILE) || (m_bFlagForProcessExit))
			throw WriteMsgAndErrcodeToFile(pstrMsg,buf);

		unsigned int leftroom = BUFFER_SIZE_LOG_FILE - m_nUsedBytesForBuffer;
		
		if(total_len > leftroom) {
			if(write(m_fd, m_pLogBuffer, m_nUsedBytesForBuffer) == -1) 
				throw CLStatus(-1,0);

			m_nUsedBytesForBuffer = 0;
		}
			

		memcpy(m_pLogBuffer + m_nUsedBytesForBuffer,pstrMsg, len_strmsg);

		m_nUsedBytesForBuffer += len_strmsg;

		memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);
		m_nUsedBytesForBuffer += len_code;

		throw CLStatus(0,0);
	}

	catch(CLStatus& s) {
		if(pthread_mutex_unlock(m_pMutexForWriteLog) != 0)
			return CLStatus(-1,0);
		return s;
	}

}

CLStatus CLLogger::WriteMsgAndErrcodeToFile(const char* pstrMsg, const char* pstrErrcode) {
	if(m_nUsedBytesForBuffer != 0) {
		if(write(m_fd, m_pLogBuffer, m_nUsedBytesForBuffer) == -1)
			return CLStatus(-1,0);
		m_nUsedBytesForBuffer = 0;
	}

	if(write(m_fd, pstrMsg, strlen(pstrMsg)) == -1)
		return CLStatus(-1,0);
	if(write(m_fd, pstrErrcode, strlen(pstrErrcode)) == -1)
		return CLStatus(-1,0);
	return CLStatus(0,0);
}

CLStatus CLLogger::WriteLogMsg(const char* pstrMsg, long lErrorCode) {
	CLLogger* pLog = GetInstance();
	if(pLog == 0) return CLStatus(-1,0);
	CLStatus s = pLog->WriteLog(pstrMsg, lErrorCode);
	if(s.IsSuccess()) 
		return CLStatus(0,0);
	else
		return CLStatus(-1,0);
}

CLStatus CLLogger::flush() {
	
	if(pthread_mutex_lock(m_pMutexForWriteLog) != 0)
		return CLStatus(-1,0);

	try{
		if(m_fd == -1)
			throw CLStatus(-1,0);
		if(m_pLogBuffer == NULL) 
			throw CLStatus(-1,0);
		if(m_nUsedBytesForBuffer == 0)
			throw CLStatus(0,0);
		ssize_t r = write(m_fd, m_pLogBuffer, m_nUsedBytesForBuffer);

		if(r == -1)
			throw CLStatus(-1, errno);
		m_nUsedBytesForBuffer = 0;
		throw CLStatus(0,0);
	}

	catch(CLStatus &s) {
		if(pthread_mutex_unlock(m_pMutexForWriteLog) != 0)
			return CLStatus(-1,0);
		return s;
	}
}

void CLLogger::OnProcessExit() {
	CLLogger* pLogger = CLLogger::GetInstance();
	if(pLogger != NULL) {
		pLogger->flush();
		pLogger->m_bFlagForProcessExit = true;
	}
}

pthread_mutex_t* CLLogger::InitMutex() {
	pthread_mutex_t* p = new pthread_mutex_t;
	if(pthread_mutex_init(p,0) != 0) {
		delete p;
		return NULL;
	}

	return p;
}
