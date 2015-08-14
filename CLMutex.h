#include "Common.h"
#include "CLLogger.h"
#include "CLStatus.h"

#ifndef _CLMUTEX_H_
#define _CLMUTEX_H_

class CLMutex {
public:
	CLMutex();
	virtual ~CLMutex();

	CLStatus Lock();
	CLStatus Unlock();
private:
	pthread_mutex_t m_Mutex;
		
};

CLMutex::CLMutex() {
	int r = pthread_mutex_init(&m_Mutex,0);
	if(r < 0)
		throw "CLMutex::CLMutex() error";
}
CLMutex::~CLMutex() {
	int r = pthread_mutex_destory(&m_Mutex);
	if(r < 0)
		CLLogger::WriteLogMsg("pthread_mutex_destroy error",0);
}
CLStatus CLMutex::Lock() {
	int r = pthread_mutex_lock(&m_Mutex);
	if(r < 0)
		return CLStatus(-1,0);
	return CLStatus(0,0);
}
CLStatus CLMutex::Unlock() {
	int r = pthread_mutex_unlock(&m_Mutex);
	if(r < 0)
		return CLStatus(-1,0);
	return CLStatus(0,0);
}
#endif
