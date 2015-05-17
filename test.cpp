#include "CLStatus.h"
#include "CLLogger.h"


int main() {
	const int n =1000;
	for(int i = 0; i < n; i++) {
		CLLogger::WriteLogMsg("nihao", 0);
	}

	CLLogger *pLogger = CLLogger::GetInstance();
	pLogger->flush();

	return  0;
}