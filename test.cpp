#include "Common.h"
#include "CLThread.h"
#include "CLPrinter.h"


int main() {

	CLExecutiveFunctionProvider* printer = new CLPrinter();
	CLExecutive* pThread = new CLThread(printer);

	cout<<"Init thread function now!!!"<<endl;
	pThread->run((void*)"I am your father");


	cout<<"Waitting for thread join"<<endl;
	pThread->waitDeath();

	cout<<"Thread joined! And byebye...."<<endl;

	return  0;
}