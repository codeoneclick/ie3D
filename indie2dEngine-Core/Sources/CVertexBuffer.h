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

#define k_NUM_REPLACEMENT_VERTEX_BUFFERS 1

struct SBone
{
    i32	m_id;
	f32 m_weigth;
};

struct SSourceVertex
{
    glm::vec3 m_position;
    glm::vec2 m_texcoord;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
    std::vector<SBone> m_bones;
};

struct SVertex
{
    glm::vec3 m_position;
    glm::vec2 m_texcoord;
    glm::u8vec4 m_normal;
    glm::u8vec4 m_tangent;
    glm::u8vec4 m_color;
};

class CVertexBuffer
{
private:
    
    static const std::string _GenerateGuid(void);
    
protected:
    
    ui32 m_size;
    std::vector<SSourceVertex> m_source;
    SVertex* m_main;
    std::map<std::string, SVertex*> m_references;
    
    ui32 m_handles[k_NUM_REPLACEMENT_VERTEX_BUFFERS];
    i32 m_index;
    GLenum m_mode;
    
    inline void _Set_Source(const std::vector<SSourceVertex>& _source)
    {
        assert(m_size == _source.size());
        m_source = _source;
    };
    
public:
    
    CVertexBuffer(ui32 _size, GLenum _mode);
    ~CVertexBuffer(void);
    
    inline const ui32 Get_Size(void) const
    {
        assert(m_size != 0);
        return m_size;
    };
    
    inline const std::vector<SSourceVertex>& Get_Source(void) const
    {
        assert(m_size == m_source.size());
        return m_source;
    };
    
    static glm::u8vec4 CompressVec3(const glm::vec3& _uncompressed);
    static glm::vec3 UncompressU8Vec4(const glm::u8vec4& _compressed);
    
    const std::string CreateReference(void);
    void DeleteReference(const std::string& _guid);
    
    SVertex* Lock(void) const;
    void Unlock(void);
    
    SVertex* Lock(const std::string& _guid) const;
    void Unlock(const std::string& _guid);
    
    void Bind(const i32* _attributes);
    void Unbind(const i32* _attributes);
};

#endif 
