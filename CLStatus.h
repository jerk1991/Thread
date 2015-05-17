#include "Common.h"

using namespace std;

#ifndef _CLSTATUS_H_
#define _CLSTATUS_H_

class CLStatus 
{
public:
	CLStatus(long lReturnCode, long lErrorCode);
	CLStatus(const CLStatus &s);
	virtual ~CLStatus();

	bool IsSuccess();

	const long& m_clReturnCode;
	const long& m_clErrorCode;
private:
	long m_lReturnCode;
	long m_lErrorCode;
};

#endif
