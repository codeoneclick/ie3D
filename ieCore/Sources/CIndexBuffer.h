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

#define K_NUM_REPLACEMENT_INDEX_BUFFERS 3

class CIndexBuffer
{
private:
    
protected:
    
    ui32 m_handles[K_NUM_REPLACEMENT_INDEX_BUFFERS];
    i32  m_index;
    
	ui16* m_data;
    ui32 m_size;
    
    GLenum m_mode;
    
public:
    
    CIndexBuffer(ui32 _size, GLenum _mode);
    ~CIndexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        return m_size;
    };
    
    inline ui16* Lock(void) const
    {
        assert(m_data != nullptr);
		return m_data;
    };
    
    void Unlock(ui32 _size = 0);
    
    void Bind(void) const;
    void Unbind(void) const;
    
};
#endif 
