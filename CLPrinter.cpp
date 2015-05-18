#include "CLPrinter.h"

CLPrinter::CLPrinter():
CLExecutiveFunctionProvider(){}
CLPrinter::~CLPrinter(){}


CLStatus CLPrinter::RunExecutiveFunction(void* pContext) {
	
	for(long i = 501; i < 1000; i++) 
		CLLogger::WriteLogMsg("In thread now",i);
	sleep(5);

	return CLStatus(0,0);
}
