//
//  CTimer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/27/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTimer.h"

#if defined(__IOS__) || defined(__OSX__)

#include <mach/mach.h>
#include <mach/mach_time.h>

#endif

ui64 CTimer::Get_TickCount(void)
{
#if defined(__IOS__) || defined(__OSX__)
    static mach_timebase_info_data_t timebaseInfo;
    uint64_t machTime = mach_absolute_time();
    if (timebaseInfo.denom == 0 )
    {
        (void)mach_timebase_info(&timebaseInfo);
    }
    uint64_t milliseconds = ((machTime / 1000000) * timebaseInfo.numer) / timebaseInfo.denom;
    return milliseconds;
#elif defined(__NDK__)
    return 0;
#elif defined(__WIN32__)
	return 0;
#endif
}

ui64 CTimer::Get_TimeInterval(const CTime& _interval_01, const CTime& _interval_02)
{
    return std::chrono::duration_cast<CMilliseconds>(_interval_01 - _interval_02).count();
}

