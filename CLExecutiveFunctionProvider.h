#include "Common.h"
#include "CLLogger.h"
#include "CLStatus.h"

#ifndef _CL_EXECUTIVE_FUNCTION_PROVIDER_
#define _CL_EXECUTIVE_FUNCTION_PROVIDER_

class CLExecutiveFunctionProvider {
public:
	CLExecutiveFunctionProvider();
	virtual ~CLExecutiveFunctionProvider();
public:
	virtual CLStatus RunExecutiveFunction(void* pContext)=0; 
};


#endif
