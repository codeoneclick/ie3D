//
//  CSVertexBuffer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSVertexBuffer_h
#define CSVertexBuffer_h

#include "HCommon.h"

class CSVertexBuffer
{
public :
    
    struct SBone
    {
        i32	m_id;
        f32 m_weigth;
    };
    
    struct SVertex
    {
        glm::vec3 m_position;
        glm::vec2 m_texcoord;
        glm::vec3 m_normal;
        glm::vec3 m_tangent;
        std::vector<SBone> m_bones;
    };
private:
    
    static const ui32 _GenerateGuid(void);
    
protected:
    
    ui32 m_size;
    SVertex* m_data;
    std::map<ui32, SVertex*> m_references;
    
public:
    
    CSVertexBuffer(CSVertexBuffer::SVertex* _data, ui32 _size);
    ~CSVertexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        assert(m_size != 0);
        return m_size;
    };
    
    inline CSVertexBuffer::SVertex* Lock(void) const
    {
        assert(m_data != nullptr);
        return m_data;
    };
    
    inline CSVertexBuffer::SVertex* Lock(ui32 _guid) const
    {
        auto iterator = m_references.find(_guid);
        assert(iterator != m_references.end());
        return iterator->second;
    };
    
    const ui32 CreateReference(void);
    void DeleteReference(ui32 _guid);
};


#endif 
