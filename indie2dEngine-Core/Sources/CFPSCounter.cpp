//
//  CFPSCounter.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/27/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CFPSCounter.h"
#include "CTimer.h"

CFPSCounter::CFPSCounter(void) :
m_totalTriagnlesPerFrame(0),
m_currentFramesPerSecond(0),
m_totalFramesPerSecond(0),
m_currentTriagnlesPerFrame(0)
{
    
}

CFPSCounter::~CFPSCounter(void)
{
    
}

void CFPSCounter::Submit(void)
{
    m_totalTriagnlesPerFrame = m_currentTriagnlesPerFrame;
    
    static CTimer::CTime oldTime;
    CTimer::CTime currentTime = CTimer::CClock::now();
    ++m_currentFramesPerSecond;
    if(CTimer::Get_TimeInterval(currentTime, oldTime) > 1000 )
    {
        oldTime = CTimer::CClock::now();
        m_totalFramesPerSecond = m_currentFramesPerSecond;
        m_currentFramesPerSecond = 0;
    }
}

void CFPSCounter::Reset(void)
{
    m_currentTriagnlesPerFrame = 0;
}