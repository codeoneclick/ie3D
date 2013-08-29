//
//  CHIndexBuffer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CHIndexBuffer_h
#define CHIndexBuffer_h

#include "HCommon.h"

#define K_NUM_REPLACEMENT_INDEX_BUFFERS 3

class CHIndexBuffer
{
private:
    
protected:
    
    ui32 m_handles[K_NUM_REPLACEMENT_INDEX_BUFFERS];
    i32  m_index;
    
	ui16* m_data;
    ui32 m_size;
    
    GLenum m_mode;
    
public:
    
    CHIndexBuffer(ui32 _size, GLenum _mode);
    ~CHIndexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        return m_size;
    };
    
    inline ui16* Lock(void) const
    {
        assert(m_data != nullptr);
		return m_data;
    };
    
    void Unlock(void);
    void Unlock(ui32 _size);
    void Unlock(ui16* _data, ui32 _size);
    
    void Bind(void) const;
    void Unbind(void) const;
};

#endif
