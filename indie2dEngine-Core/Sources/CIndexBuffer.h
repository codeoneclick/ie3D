//
//  CIndexBuffer.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CIndexBuffer_h
#define CIndexBuffer_h

#include "HCommon.h"

#define K_NUM_REPLACEMENT_INDEX_BUFFERS 1

class CIndexBuffer
{
private:
    
protected:
    
    ui32 m_handles[K_NUM_REPLACEMENT_INDEX_BUFFERS];
    i32  m_currentHandleIndex;
	ui16* m_data;
    GLenum m_mode;
    ui32 m_size;
    
public:
    
    CIndexBuffer(ui32 _size, GLenum _mode);
    ~CIndexBuffer(void);
    
    inline const ui32 Get_NumIndexes(void)
    {
        return m_size;
    };
    
    inline ui16* Lock(void)
    {
        assert(m_data != nullptr);
		return m_data;
    };
    
    void Unlock(void);
    void Unlock(ui32 _size);
    
    void Bind(void);
    void Unbind(void);
    
};

#endif 
