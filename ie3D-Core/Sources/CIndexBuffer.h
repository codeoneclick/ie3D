//
//  CIndexBuffer.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CIndexBuffer_h
#define CIndexBuffer_h

#include "IResource.h"
#include "HDeclaration.h"

#define K_NUM_REPLACEMENT_INDEX_BUFFERS 3

class CIndexBuffer : public IResourceData
{
private:
    
protected:
    
    ui32 m_handles[K_NUM_REPLACEMENT_INDEX_BUFFERS];
    i32  m_index;
    
	ui16* m_data;
    ui32 m_size;
    
    GLenum m_mode;
    
public:
    
    CIndexBuffer(ui32 size, GLenum mode);
    ~CIndexBuffer(void);
    
    ui32 getSize(void) const;
    
    ui16* lock(void) const;
    void unlock(ui32 size = 0);
    
    void bind(void) const;
    void unbind(void) const;
};
#endif 
