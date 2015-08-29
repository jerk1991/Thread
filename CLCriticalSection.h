#include "CLMutex.h"

#ifndef _CLCRITICALSECTION_H_
#define _CLCRITICALSECTION_H_

class CLCriticalSection {
public:
	CLCriticalSection(CLMutex* p_Mutex) {
		if(p_Mutex == 0)
			throw "CLCriticalSection::CLCriticalSection() error";
		m_pMutex = p_Mutex;
		CLStatus s = m_pMutex->Lock();
		if(!s.IsSuccess()) {
			throw "CLCriticalSection::CLCriticalSection() m_pMutex->lock() error";
		}
		
	}
	virtual ~CLCriticalSection() {
		CLStatus s = m_pMutex->Unlock();
		if(!s.IsSuccess()) {
			CLLogger::WriteLogMsg("~CLCriticalSection() m_pMutex->Unlock() error",0);
		}
	}
private:
	CLMutex *m_pMutex;
};

#endif
