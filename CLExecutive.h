#include "Common.h"
#include "CLLogger.h"
#include "CLStatus.h"
#include "CLExecutiveFunctionProvider.h"

#ifndef _CL_EXECUTIVE_H_
#define _CL_EXECUTIVE_H_

class CLExecutive {
	public:
		explicit CLExecutive(CLExecutiveFunctionProvider* pExecutiveFunctionProvider)
		{
			if(pExecutiveFunctionProvider == NULL) {
				CLLogger::WriteLogMsg("In CLExecutive::CLExecutive(),pExecutiveFunctionProvider is NULL",-1);
			}
			m_pExecutiveFunctionProvider = pExecutiveFunctionProvider;
		}
		virtual ~CLExecutive() {
			delete m_pExecutiveFunctionProvider;
		}

		virtual CLStatus Run(void *pContex=0)=0;
		virtual CLStatus WaitforDeath()=0;
	protected:
		CLExecutiveFunctionProvider* m_pExecutiveFunctionProvider;
};

#endif
