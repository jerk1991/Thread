#include "CLPrinter.h"

CLPrinter::CLPrinter():
CLExecutiveFunctionProvider(){}
CLPrinter::~CLPrinter(){}


CLStatus CLPrinter::RunExecutiveFunction(void* pContext) {
	char * m_pContex = (char*)pContext;
	
	sleep(5);
	if(m_pContex == NULL) 
		return CLStatus(0,0);
	if(strncmp("end", m_pContex, 3) != 0) {
		unsigned len = strlen(m_pContex);
		cout<<"You put "<<len<<" characters"<<endl;

		return CLStatus(0,0);
	}

	return CLStatus(0,0);
}
