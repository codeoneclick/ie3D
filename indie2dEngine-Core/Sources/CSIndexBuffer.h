//
//  CSIndexBuffer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSIndexBuffer_h
#define CSIndexBuffer_h

#include "HCommon.h"

class CSIndexBuffer
{
private:
    
     static const ui32 _GenerateGuid(void);
    
protected:
    
	ui16* m_data;
    ui32 m_size;
    std::map<ui32, ui16*> m_references;
    
public:
    
    CSIndexBuffer(ui16* _data, ui32 _size);
    ~CSIndexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        return m_size;
    };
    
    inline ui16* Lock(void) const
    {
        assert(m_data != nullptr);
		return m_data;
    };
    
    inline ui16* Lock(ui32 _guid) const
    {
        auto iterator = m_references.find(_guid);
        assert(iterator != m_references.end());
        return iterator->second;
    };
    
    const ui32 CreateReference(void);
    void DeleteReference(ui32 _guid);
};


#endif 
