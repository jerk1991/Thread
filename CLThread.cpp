#include "CLThread.h"

CLThread::CLThread(CLExecutiveFunctionProvider* pExecutiveFunctionProvider, bool bWaitforDeath = true):
CLExecutive(pExecutiveFunctionProvider) {
		m_bThreadCreated = false;
		m_bWaitforDeath = bWaitforDeath;
	}

CLThread::~CLThread() {
	delete m_pExecutiveFunctionProvider;	
}

CLStatus CLThread::Run(void *pContex) {
	if(m_bThreadCreated)
		return CLStatus(-1,0);
	m_pContex = pContex;
	int r = pthread_create(&m_threadID, 0, StartFunctionOfThread, this);
	if(r != 0) {
		CLLogger::WriteLogMsg("In CLThread::Run(), pthread_create error", r);
		delete this;
		return CLStatus(-1,0);
	}
	m_bThreadCreated = true;
	if(!m_bWaitforDeath) {
		r = pthread_detach(m_threadID);
		if(r != 0)
		{
			//............
		}
	}

	return CLStatus(0,0);
}

CLStatus CLThread::WaitforDeath() {
	if(!m_bThreadCreated || !m_bWaitforDeath)
		return CLStatus(-1,0);
	int r = pthread_join(m_threadID, 0);
	if(r != 0) {
		CLLogger::WriteLogMsg("In CLThread::WaitforDeath(), pthread_create error",r);
		return CLStatus(-1,0);
	}

	delete this;

	return CLStatus(0,0);
}

void* CLThread::StartFunctionOfThread(void *pThis) {
	CLThread* pThreadThis = (CLThread*)pThis;

	pThreadThis->m_pExecutiveFunctionProvider->RunExecutiveFunction(pThreadThis->m_pContex);

	if(!(pThreadThis->m_bThreadCreated))
		delete pThreadThis;

	return 0;
}
