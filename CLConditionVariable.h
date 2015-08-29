#include "CLMutex.h"

#ifndef _CLCONDITIONVARIABLE_H_
#define _CLCONDITIONVARIABLE_H_

class CLConditionVariable {
public:
    CLConditionVariable();
    virtual ~CLConditionVariable();
    
    CLStatus Wait(CLMutex* pMutex);
    CLStatus Wakeup();
    CLStatus WakeupAll();
    
private:
    pthread_cond_t m_ConditionVariable;
};

CLConditionVariable::CLConditionVariable() {
    int r = pthread_cond_init(&m_ConditionVariable,0);
    if (r < 0) {
        CLLogger::WriteLogMsg("In CLConditionVariable() pthread_cond_init() error", 0);
        throw "In CLConditionVariable() pthread_cond_init() error";
    }
}
CLConditionVariable::~CLConditionVariable() {
    int r = pthread_cond_destroy(&m_ConditionVariable);
    if (r < 0) {
        CLLogger::WriteLogMsg("In CLConditionVariable::~CLConditionVariable() pthread_cond_destroy() error", 0);
    }
}
CLStatus CLConditionVariable::Wait(CLMutex* pMutex) {
    int r = pthread_cond_wait(&m_ConditionVariable,pMutex->GetMutexPointer());
    if (r < 0) {
        CLLogger::WriteLogMsg("In CLConditionVariable::Wait() pthread_cond_wait() error",0);
        return CLStatus(-1,0);
    }
    return CLStatus(0,0);
}
CLStatus CLConditionVariable::Wakeup() {
    int r = pthread_cond_signal(&m_ConditionVariable);
    if (r < 0) {
        CLLogger::WriteLogMsg("In CLConditionVariable::Wakeup(),pthread_cond_signal() error",0);
        return CLStatus(-1,0);
    }
    return CLStatus(0,0);
}
CLStatus CLConditionVariable::WakeupAll() {
    int r = pthread_cond_broadcast(&m_ConditionVariable);
    if (r < 0) {
        CLLogger::WriteLogMsg("In CLConditionVariable::WakeupAll(),pthread_cond_broadcast() error",0);
        return CLStatus(-1,0);
    }
    return CLStatus(0,0);
}

#endif

