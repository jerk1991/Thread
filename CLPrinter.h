#include "Common.h"
#include "CLStatus.h"
#include "CLLogger.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLExecutive.h"

#ifndef _CL_PRINTER_H_
#define _CL_PRINTER_H_

class CLPrinter: public CLExecutiveFunctionProvider
{
public:
	CLPrinter();
	virtual ~CLPrinter();

	virtual CLStatus RunExecutiveFunction(void* pContext);
};


#endif
