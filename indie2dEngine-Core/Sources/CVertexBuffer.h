//
//  CVertexBuffer.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CVertexBuffer_h
#define CVertexBuffer_h

#include "HCommon.h"

#define k_NUM_REPLACEMENT_VERTEX_BUFFERS 3

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

struct SHardwareVertex
{
    glm::vec3    m_position;
    glm::u16vec2 m_texcoord;
    glm::u8vec4  m_normal;
    glm::u8vec4  m_tangent;
    glm::u8vec4  m_color;
    glm::u8vec4  m_extra;
};

class CVertexBuffer
{
private:
    
protected:
    
    ui32 m_size;
    SHardwareVertex* m_data;
    
    ui32 m_handles[k_NUM_REPLACEMENT_VERTEX_BUFFERS];
    i32 m_index;
    
    GLenum m_mode;
    
public:
    
    CVertexBuffer(ui32 _size, GLenum _mode);
    ~CVertexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        assert(m_size != 0);
        return m_size;
    };
    
    static glm::u8vec4 CompressVec3(const glm::vec3& _uncompressed);
    static glm::vec3 UncompressU8Vec4(const glm::u8vec4& _compressed);
    
    static glm::u16vec2 CompressVec2(const glm::vec2& _uncompressed);
    static glm::vec2 UncompressU16Vec2(const glm::u16vec2& _compressed);
    
    SHardwareVertex* Lock(void) const;
    void Unlock(ui32 _size = 0);
    
    void Bind(const i32* _attributes);
    void Unbind(const i32* _attributes);
};

#endif 
