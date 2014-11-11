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

class CIndexBuffer : public IResourceData
{
private:
    
protected:
    
    ui32 m_handle;
    bool m_isDataUploaded;
    
	ui16* m_data;
    ui32 m_allocatedSize;
    ui32 m_usedSize;
    
    GLenum m_mode;
    
public:
    
    CIndexBuffer(ui32 sizeToAllocate, GLenum mode);
    ~CIndexBuffer(void);
    
    ui32 getAllocatedSize(void) const;
    ui32 getUsedSize(void) const;
    
    ui16* lock(void) const;
    void unlock(ui32 sizeToUse = 0);
    
    void bind(void) const;
    void unbind(void) const;
};
#endif 
