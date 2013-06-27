//
//  CFPSCounter.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/27/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CFPSCounter_h
#define CFPSCounter_h

#include "HCommon.h"

class CFPSCounter final
{
private:
    
protected:
    
    ui32 m_currentTriagnlesPerFrame;
    ui32 m_totalTriagnlesPerFrame;
    ui32 m_currentFramesPerSecond;
    ui32 m_totalFramesPerSecond;
    
public:
    
    CFPSCounter(void);
    ~CFPSCounter(void);
    
    void Submit(void);
    void Reset(void);
    
    inline ui32 Get_FramesPerSecond(void)
    {
        return m_totalFramesPerSecond;
    };
    
    inline ui32 Get_TrianglesPerSecond(void)
    {
        return m_totalTriagnlesPerFrame;
    };
    
    inline void Inc_TrianglesCount(ui32 _value)
    {
        m_currentTriagnlesPerFrame += _value;
    };
};

#endif 
