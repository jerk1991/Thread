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
        p->Set();
        //.........
    }
};

int main() {
    CLEvent* pEvent = new CLEvent;
    CLExecutiveFunctionProvider *myfunction = new CLMyFunction();
    CLExecutive *pThread = new CLThread(myfunction);
    pThread->Run((void*)pEvent);
    
    pEvent->wait();
    pThread->waitDeath();
    return 0;
    
}