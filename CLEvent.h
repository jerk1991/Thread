#include "CLConditionVariable.h"
#include "CLCriticalSection.h"

#ifndef _CLEVENT_H_
#define _CLEVENT_H_

class CLEvent {
    
public:
    CLEvent();
    virtual ~CLEvent();
    CLStatus Set();
    CLStatus Wait();
private:
    CLMutex m_Mutex;
    CLConditionVariable m_Cond;
    int m_Flag;
};
CLEvent::CLEvent() {
    m_Flag = 0;
}
CLEvent::~CLEvent() {
    
}
CLStatus CLEvent::Set() {
    try {
        CLCriticalSection cs(&m_Mutex);
        
        m_Flag = 1;
    } catch (const char* s) {
        //..........
    }
    CLStatus s = m_Cond.Wakeup();
    //..........
    return CLStatus(0,0);
}
CLStatus CLEvent::Wait() {
    try {
        CLCriticalSection cs(&m_Mutex);
        while (m_Flag == 0) {
            CLStatus s = m_Cond.Wait(&m_Mutex);
        }
        //将m_Flag复位为0，复位的好处是，下次调用Wait函数继续等待事件变成有信号状态。
        m_Flag = 0;
        
    } catch (const char* s) {
        //.....
    }
    return CLStatus(0,0);
}
#endif