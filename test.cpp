//
//  test.cpp
//  
//
//  Created by 苏杭 on 15/8/29.
//
//

#include <stdio.h>
#include "CLThread.h"
#include "CLEvent.h"

class CLMyFunction:public CLExecutiveFunctionProvider
{
public:
    CLMyFunction() {}
    virtual ~CLMyFunction() {}
    
    virtual CLStatus RunExecutiveFunction(void * pContex) {
        
        CLEvent *p = (CLEvent*)pContex;
		cout<<"Condition test"<<endl;
		cout<<"call Set()"<<endl;
        p->Set();
        //.........
		//sleep(5);
		return CLStatus(0,0);
    }
};

int main() {
    CLEvent* pEvent = new CLEvent;
    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);
    pThread->run((void*)pEvent);
   // sleep(5);
	cout<<"wait for condition"<<endl;
    pEvent->Wait();
	cout<<"Wake up"<<endl;
    pThread->waitDeath();
    return 0;
    
}
