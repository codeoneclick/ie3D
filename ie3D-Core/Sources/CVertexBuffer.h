//
//  CVertexBuffer.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CVertexBuffer_h
#define CVertexBuffer_h

#include "IResource.h"
#include "HDeclaration.h"

#define k_NUM_REPLACEMENT_VERTEX_BUFFERS 3

struct SBoneData
{
    i32	m_id;
	f32 m_weigth;
};

struct SVertexData
{
    glm::vec3 m_position;
    glm::vec2 m_texcoord;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;
    std::vector<SBoneData> m_bones;
};

struct SAttributeVertex
{
    glm::vec3    m_position;
    glm::u16vec2 m_texcoord;
    glm::u8vec4  m_normal;
    glm::u8vec4  m_tangent;
    glm::u8vec4  m_color;
    glm::u8vec4  m_extra;
};

class CVertexBuffer final : public IResourceData
{
private:
    
protected:
    
    ui32 m_vao;
    ui32 m_handles[k_NUM_REPLACEMENT_VERTEX_BUFFERS];
    i32 m_index;
    
    SAttributeVertex* m_data;
    ui32 m_allocatedSize;
    ui32 m_usedSize;
    
    GLenum m_mode;
    
public:
    
    CVertexBuffer(ui32 sizeToAllocate, GLenum mode);
    ~CVertexBuffer(void);
    
    static glm::u8vec4 compressVec3(const glm::vec3& uncompressed);
    static glm::vec3 uncompressU8Vec4(const glm::u8vec4& compressed);
    
    static glm::u16vec2 compressVec2(const glm::vec2& uncompressed);
    static glm::vec2 uncompressU16Vec2(const glm::u16vec2& compressed);
    
    ui32 getAllocatedSize(void) const;
    ui32 getUsedSize(void) const;
    
    SAttributeVertex* lock(void) const;
    void unlock(ui32 sizeToUse = 0);
    
    void bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
    void unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
};

#endif 
