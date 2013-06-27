//
//  CTimer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/27/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTimer_h
#define CTimer_h

#include "HCommon.h"

class CTimer
{
private:
    
protected:
    
public:
    
    typedef std::chrono::high_resolution_clock CClock;
    typedef std::chrono::milliseconds CMilliseconds;
    typedef CClock::time_point CTime;
    
    static ui64 Get_TickCount(void);
    static ui64 Get_TimeInterval(const CTime& _interval_01, const CTime& _interval_02);
};

#endif
