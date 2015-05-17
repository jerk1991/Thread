#include "CLThread.h"

CLThread::CLThread(CLExecutiveFunctionProvider* pExecutiveFunctionProvider):
CLExecutive(pExecutiveFunctionProvider) {}

CLThread::~CLThread() {}

CLStatus CLThread::run(void *pContex) {
	m_pContex = pContex;
	int r = pthread_create(&m_threadID, 0, StartFunctionOfThread, this);
	if(r != 0) {
		CLLogger::WriteLogMsg("In CLThread::run(), pthread_create error", r);
		return CLStatus(-1,0);
	}

	return CLStatus(0,0);
}

CLStatus CLThread::waitDeath() {
	int r = pthread_join(m_threadID, 0);
	if(r != 0) {
		CLLogger::WriteLogMsg("In CLThread::waitDeath(), pthread_create error",r);
		return CLStatus(-1,0);
	}

	return CLStatus(0,0);
}

void* CLThread::StartFunctionOfThread(void *pThis) {
	CLThread* pThreadThis = (CLThread*)pThis;

	pThreadThis->m_pExecutiveFunctionProvider->RunExecutiveFunction(pThreadThis->m_pContex);

	return 0;
}