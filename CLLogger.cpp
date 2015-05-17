#include "CLLogger.h"

#define LOG_FILE_NAME "CLLogger.text"
#define MAX_SIZE 256
#define BUFFER_SIZE_LOG_FILE 4096

CLLogger* CLLogger::m_pLog = NULL;

CLLogger::CLLogger() {
	m_fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	m_nUsedBytesForBuffer = 0;
}

CLLogger::~CLLogger() {
	if(m_fd != -1) 
		close(m_fd);
	delete [] m_pLogBuffer;
}

CLLogger::CLLogger(const CLLogger& s) {
	
	m_fd = s.m_fd;
	m_pLog = s.m_pLog;
	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	strcpy(m_pLogBuffer, s.m_pLogBuffer);
	m_nUsedBytesForBuffer = s.m_nUsedBytesForBuffer;
}

CLLogger& CLLogger::operator=(const CLLogger& s) {
	
	if(this == &s) return *this;
	
	delete [] m_pLogBuffer;
	m_pLogBuffer = NULL;
	
	m_fd = s.m_fd;
	m_pLog = s.m_pLog;
	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	strcpy(m_pLogBuffer, s.m_pLogBuffer);
	m_nUsedBytesForBuffer = s.m_nUsedBytesForBuffer;

	return *this; 
}

CLLogger* CLLogger::GetInstance() {
	if(m_pLog == 0) {
		m_pLog = new CLLogger;

		if(atexit(CLLogger::OnProcessExit) != 0) {
			if(m_pLog != 0) {
				m_pLog->WriteLog("In CLLogger::GetInstance(), atexit() error", errno);
				m_pLog->flush();
			}
		}
	}

	return m_pLog;
}

CLStatus CLLogger::WriteLog(const char* pstrMsg, long lErrorCode) {
	if(pstrMsg == NULL) return CLStatus(-1,0);

	if(strlen(pstrMsg) == 0) return CLStatus(-1,0);

	if(m_fd == -1) return CLStatus(-1,0);

	unsigned int leftroom = BUFFER_SIZE_LOG_FILE - m_nUsedBytesForBuffer;
	unsigned int len_strmsg = strlen(pstrMsg);

	char buf[MAX_SIZE];
	snprintf(buf, MAX_SIZE, "Error code: %ld\r\n", lErrorCode);
	unsigned int len_code = strlen(buf);

	unsigned int total_len = len_strmsg + len_code;

	if(total_len > BUFFER_SIZE_LOG_FILE) {
		if(m_fd == -1) return CLStatus(-1,0);
		CLStatus s  = flush();
		if(s.IsSuccess()) {
			ssize_t r = write(m_fd, pstrMsg, len_strmsg);
			if(r == -1) return CLStatus(-1,0);

			r = write(m_fd, buf, len_code);
			if(r == -1) return CLStatus(-1,0);

			return CLStatus(0,0);
		}
		return CLStatus(-1,0);
	}

	if(total_len > leftroom) {
		CLStatus s = flush();
		if(!s.IsSuccess())
			return CLStatus(-1,0);
	}

	memcpy(m_pLogBuffer + m_nUsedBytesForBuffer,pstrMsg, len_strmsg);

	m_nUsedBytesForBuffer += len_strmsg;

	memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);
	m_nUsedBytesForBuffer += len_code;

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
	if(m_fd == -1)
		return CLStatus(-1,0);
	if(m_pLogBuffer == NULL) 
		return CLStatus(-1,0);

	if(m_nUsedBytesForBuffer == 0)
		return CLStatus(0,0);
	ssize_t r = write(m_fd, m_pLogBuffer, m_nUsedBytesForBuffer);

	if(r == -1)
		return CLStatus(-1, errno);
	m_nUsedBytesForBuffer = 0;

	return CLStatus(0,0);
}

void CLLogger::OnProcessExit() {
	CLLogger* pLogger = CLLogger::GetInstance();
	if(pLogger != NULL) 
		pLogger->flush();
}
