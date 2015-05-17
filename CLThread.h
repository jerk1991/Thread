#include "Common.h"
#include "CLLogger.h"
#include "CLStatus.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLExecutive.h"

#ifndef _CL_THREAD_H_
#define _CL_THREAD_H_

class CLThread : public CLExecutive
{

public:
	explicit CLThread(CLExecutiveFunctionProvider* pExecutiveFunctionProvider);
	virtual ~CLThread();

	virtual CLStatus run(void *pContex=0);

	virtual CLStatus waitDeath();

private:
	static void* StartFunctionOfThread(void *pThis);
private:
	void* m_pContex;
	pthread_t m_threadID;
};

#endif