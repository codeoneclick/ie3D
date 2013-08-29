//
//  CHVertexBuffer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CHVertexBuffer_h
#define CHVertexBuffer_h

#include "HCommon.h"
#include "CSVertexBuffer.h"

#define k_NUM_REPLACEMENT_VERTEX_BUFFERS 3

class CHVertexBuffer
{
public :
    
    struct SVertex
    {
        glm::vec3 m_position;
        glm::vec2 m_texcoord;
        glm::u8vec4 m_normal;
        glm::u8vec4 m_tangent;
        glm::u8vec4 m_color;
    };
    
private:
    
protected:
    
    CHVertexBuffer::SVertex* m_data;
    ui32 m_size;
    
    ui32 m_handles[k_NUM_REPLACEMENT_VERTEX_BUFFERS];
    i32 m_index;
    
    GLenum m_mode;
    
public:
    
    CHVertexBuffer(ui32 _size, GLenum _mode);
    ~CHVertexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        assert(m_size != 0);
        return m_size;
    };
        
    static glm::u8vec4 CompressVec3(const glm::vec3& _uncompressed);
    static glm::vec3 UncompressU8Vec4(const glm::u8vec4& _compressed);
    
    CHVertexBuffer::SVertex* Lock(void) const;
    
    void Unlock(void);
    void Unlock(ui32 _size);
    void Unlock(CSVertexBuffer::SVertex* _data, ui32 _size);
    
    void Bind(const i32* _attributes) const;
    void Unbind(const i32* _attributes) const;
};


#endif 
