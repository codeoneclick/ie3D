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
    glm::vec3 m_position;
    glm::uint32 m_texcoord;
    glm::uint32 m_normal;
    glm::uint32 m_tangent;
    glm::u8vec4 m_color;
    glm::u8vec4 m_extra;
    
    SAttributeVertex(void) = default;
    ~SAttributeVertex(void) = default;
};

class CVertexBuffer final : public IResourceData
{
private:
    
protected:
    
    ui32 m_handle;
    bool m_isMMAP;
    
    SAttributeVertex* m_data;
    ui32 m_allocatedSize;
    ui32 m_usedSize;
    
    GLenum m_mode;
    
public:
    
    CVertexBuffer(ui32 sizeToAllocate, GLenum mode, SAttributeVertex* mmap = nullptr);
    ~CVertexBuffer(void);
    
    ui32 getAllocatedSize(void) const;
    ui32 getUsedSize(void) const;
    
    SAttributeVertex* lock(void) const;
    void unlock(ui32 sizeToUse = 0);
    
    void bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
    void unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
};

#endif 
